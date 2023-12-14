#include <linux/cdev.h>    // cdev_add, cdev_init
#include <linux/uaccess.h> // copy_to_user
#include <linux/module.h>  // module_init, GPL
#include <linux/spi/spi.h> // spi_sync,
#include <linux/of.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#define MAXLEN 32
#define MODULE_DEBUG 1 // Enable/Disable Debug messages
/* Char Driver Globals */
static struct spi_driver spi_drv_spi_driver;
struct file_operations spi_drv_fops;
static struct class *spi_drv_class;
static dev_t devno;
static struct cdev spi_drv_cdev;
/* Definition of SPI devices */
struct mcp4802dev
{
  struct spi_device *spi; // Pointer to SPI device
  int channel;            // channel, ex. adc ch 0
  int gpio_dac_ld;        // NEW: gpio pin for dac load
  int removed;            // NEW: flag to indicate device is removed
};
/* Array of SPI devices */
/* Minor used to index array */
struct mcp4802dev spi_devs[4];
const int spi_devs_len = 4;  // Max nbr of devices
static int spi_devs_cnt = 0; // Nbr devices present
/* Macro to handle Errors */
#define ERRGOTO(label, ...) \
  {                         \
    printk(__VA_ARGS__);    \
    goto label;             \
  }                         \
  while (0)
/**********************************************************
 * CHARACTER DRIVER METHODS
 **********************************************************/
/*
 * Character Driver Module Init Method
 */
static int __init spi_drv_init(void)
{
  int err = 0;
  printk("spi_drv driver initializing\n");
  /* Allocate major number and register fops*/
  err = alloc_chrdev_region(&devno, 0, 255, "spi_drv driver");
  if (MAJOR(devno) <= 0)
    ERRGOTO(err_no_cleanup, "Failed to register chardev\n");
  printk(KERN_ALERT "Assigned major no: %i\n", MAJOR(devno));
  cdev_init(&spi_drv_cdev, &spi_drv_fops);
  err = cdev_add(&spi_drv_cdev, devno, 255);
  if (err)
    ERRGOTO(err_cleanup_chrdev, "Failed to create class");
  /* Polulate sysfs entries */
  spi_drv_class = class_create(THIS_MODULE, "spi_drv_class");
  if (IS_ERR(spi_drv_class))
    ERRGOTO(err_cleanup_cdev, "Failed to create class");
  /* Register SPI Driver */
  /* THIS WILL INVOKE PROBE, IF DEVICE IS PRESENT!!! */
  err = spi_register_driver(&spi_drv_spi_driver);
  if (err)
    ERRGOTO(err_cleanup_class, "Failed SPI Registration\n");
  /* Success */
  return 0;
  /* Errors during Initialization */
err_cleanup_class:
  class_destroy(spi_drv_class);
err_cleanup_cdev:
  cdev_del(&spi_drv_cdev);
err_cleanup_chrdev:
  unregister_chrdev_region(devno, 255);
err_no_cleanup:
  return err;
}
/*
 * Character Driver Module Exit Method
 */
static void __exit spi_drv_exit(void)
{
  printk("spi_drv driver Exit\n");
  spi_unregister_driver(&spi_drv_spi_driver);
  class_destroy(spi_drv_class);
  cdev_del(&spi_drv_cdev);
  unregister_chrdev_region(devno, 255);
}

/*
 * Adapted from SPI write function found in HAL lecture 7 slides numbers 22
 */
int spi_drv_write_dac(struct mcp4802dev *spidev, int millivolts)
{
  int err;
  struct spi_transfer t[1]; /* NEW: One transfers */
  struct spi_message m;
  memset(t, 0, sizeof(t)); /* Init Memory */
  spi_message_init(&m);    /* Init Msg */
  m.spi = spidev->spi;     /* Use current SPI I/F */
  u8 buf=0x00;             /* initialise buffer*/
  u8 dataValue = millivolts; // Convert millivolts to 8-bit data value (0-255)
  if (dataValue==1)
  {buf = 0b11111111;} /*Lås*/
  if(dataValue==0)
  {buf = 0b11000000;} /*Åben*/
  pr_info("buf: %d, milivolts = %d, data val = %d\n", buf, millivolts, dataValue);
  /* NEW: Prepare buffer for transmit */
  t[0].tx_buf = &buf;
  t[0].rx_buf = NULL; /* Recieve No data */
  t[0].len = 2;       /* Transfer Size in Bytes */
  spi_message_add_tail(&t[0], &m);
  err = spi_sync(m.spi, &m); /* Blocking Transmit */
  if (err != 0)
  {
    return err;
  }
  /* NEW: Pulse dac_ld gpio to load DAC */
  if (spidev->gpio_dac_ld > 0)
  {
    gpio_set_value(spidev->gpio_dac_ld, 0);
    mdelay(10);
    gpio_set_value(spidev->gpio_dac_ld, 1);
  }
  return 0;
}

/*
 * Character Driver Write File Operations Method
 */
ssize_t spi_drv_write(struct file *filep, const char __user *ubuf,
                      size_t count, loff_t *f_pos)
{
  int err, minor, len, value;
  char kbuf[MAXLEN];
  minor = iminor(filep->f_inode);
  printk(KERN_ALERT "Writing to spi_drv [Minor] %i \n", minor);
  /* Limit copy length to MAXLEN allocated andCopy from user */
  len = count < MAXLEN ? count : MAXLEN;
  if (copy_from_user(kbuf, ubuf, len))
    return -EFAULT;
  /* Pad null termination to string */
  kbuf[len] = '\0';
  if (MODULE_DEBUG)
    printk("string from user: %s\n", kbuf);
  /* Convert sting to int */
  sscanf(kbuf, "%i", &value);
  if (MODULE_DEBUG)
    printk("value %i\n", value);
  /*
    Do something with value ....
  */
  err = spi_drv_write_dac(&spi_devs[minor], value);
  if (err != 0)
  {
    printk(KERN_ERR "Failed to write to spi device. retval = %d\n", err);
    return -1;
  }
  /* Legacy file ptr f_pos. Used to support
   * random access but in char drv we dont!
   * Move it the length actually  written
   * for compability */
  *f_pos += len;
  /* return length actually written */
  return len;
}
/*
 * Character Driver File Operations Structure
 */
struct file_operations spi_drv_fops =
    {
        .owner = THIS_MODULE,
        .write = spi_drv_write,
};
/**********************************************************
 * LINUX DEVICE MODEL METHODS (spi)
 **********************************************************/
/*
 * spi_drv Probe
 * Called when a device with the name "spi_drv" is
 * registered.
 */
static int spi_drv_probe(struct spi_device *sdev)
{
  int err = 0;
  struct device *spi_drv_device;
  struct device_node *np = sdev->dev.of_node; // Device tree node ptr
  printk(KERN_DEBUG "New SPI device: %s using chip select: %i\n",
         sdev->modalias, sdev->chip_select);
  /* NEW: Try read DAC_LD gpio pin number from device tree. If not found, then dont use it */
  u32 dacLoadPin;
  err = of_property_read_u32(np, "dac-ld-gpio", &dacLoadPin);
  if (err == -EINVAL)
  {
    dacLoadPin = -1;
  }
  else if (err != 0)
  {
    dev_err(&(sdev->dev), "Failed to read dac-ld-gpio: got retval %d\n", err);
    return -EINVAL;
  }
  /* NEW: Request dac_ld gpio if applicable (valid dacLoadPin above 0) and not already used by another spi device */
  if (dacLoadPin > 0)
  {
    int dac_ld_reserved = 0;
    for (int i = 0; i < spi_devs_cnt; i++)
    {
      if (spi_devs[i].gpio_dac_ld == dacLoadPin)
      {
        dac_ld_reserved = 1;
      }
    }
    if (!dac_ld_reserved)
    {
      err = gpio_request(dacLoadPin, "gpio_dac_ld");
      if (err)
      {
        printk(KERN_ERR "FAILED TO REQUEST GPIO %d\n", dacLoadPin);
        return -EINVAL;
      }
      /* Set dac_ld gpio to output and default to high */
      err = gpio_direction_output(dacLoadPin, 0);
      if (err)
      {
        printk(KERN_ERR "FAILED TO SET DIRECTION ON GPIO %d\n", dacLoadPin);
        return -EINVAL;
      }
    }
  }
  /* Check we are not creating more
     devices than we have space for */
  if (spi_devs_cnt > spi_devs_len)
  {
    printk(KERN_ERR "Too many SPI devices for driver\n");
    return -ENODEV;
  }
  /* Configure bits_per_word, always 8-bit for RPI!!! */
  sdev->bits_per_word = 8;
  err = spi_setup(sdev);
  if (err != 0)
  {
    printk(KERN_ERR "FAILED TO SETUP SPI DEVICE. retval = %d\n", err);
    return -ENODEV;
  }
  /* Create devices, populate sysfs and
     active udev to create devices in /dev */
  /* We map spi_devs index to minor number here */
  spi_drv_device = device_create(spi_drv_class, NULL,
                                 MKDEV(MAJOR(devno), spi_devs_cnt),
                                 NULL, "spi_drv%d", spi_devs_cnt);
  if (IS_ERR(spi_drv_device))
    printk(KERN_ALERT "FAILED TO CREATE DEVICE\n");
  else
    printk(KERN_ALERT "Using spi_devs%i on major:%i, minor:%i\n",
           spi_devs_cnt, MAJOR(devno), spi_devs_cnt);
  /* Update local array of SPI devices */
  spi_devs[spi_devs_cnt].spi = sdev;
  spi_devs[spi_devs_cnt].gpio_dac_ld = dacLoadPin; // NEW: save dac_ld gpio pin
  spi_devs[spi_devs_cnt].channel = 0x00;           // channel address
  spi_devs[spi_devs_cnt].removed = 0;              // NEW: device not removed
  ++spi_devs_cnt;
  return err;
}
/*
 * spi_drv Remove
 * Called when the device is removed
 * Can deallocate data if needed
 */
static int spi_drv_remove(struct spi_device *sdev)
{
  int its_minor = 0;
  printk(KERN_ALERT "Removing spi device\n");
  // NEW: Find the minor number of the device being removed
  its_minor = MINOR(sdev->dev.devt);
  /* Destroy devices created in probe() */
  device_destroy(spi_drv_class, MKDEV(MAJOR(devno), its_minor));
  /* NEW: Free dac_ld gpio if applicable (valid dac_ld_gpio above 0) not used by another device */
  if (spi_devs[its_minor].gpio_dac_ld > 0)
  {
    int dac_ld_still_used = 0;
    for (int i = 0; i < spi_devs_cnt; i++)
    {
      if (!spi_devs[i].removed && spi_devs[i].gpio_dac_ld == spi_devs[its_minor].gpio_dac_ld)
      {
        dac_ld_still_used = 1;
      }
    }
    if (!dac_ld_still_used)
    {
      gpio_free(spi_devs[its_minor].gpio_dac_ld);
    }
  }
  spi_devs[its_minor].removed = 1;
  return 0;
}
/*
 * spi Driver Struct
 * Holds function pointers to probe/release
 * methods and the name under which it is registered
 */
static const struct of_device_id of_spi_drv_spi_device_match[] = {
    {
        .compatible = "ase, spi_drv",
    },
    {},
};
static struct spi_driver spi_drv_spi_driver = {
    .probe = spi_drv_probe,
    .remove = spi_drv_remove,
    .driver = {
        .name = "spi_drv",
        .bus = &spi_bus_type,
        .of_match_table = of_spi_drv_spi_device_match,
        .owner = THIS_MODULE,
    },
};






/**********************************************************
 * GENERIC LINUX DEVICE DRIVER STUFF
 **********************************************************/
/*
 * Assignment of module init/exit methods
 */
module_init(spi_drv_init);
module_exit(spi_drv_exit);
/*
 * Assignment of author and license
 */
MODULE_AUTHOR("Gruppe Prj");
MODULE_LICENSE("GPL");