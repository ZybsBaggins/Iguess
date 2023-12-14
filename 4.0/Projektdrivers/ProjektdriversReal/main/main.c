#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define SW2_DEVICE "/home/root/dev/driver1"   // Adjust the device path based on your setup
#define SPI_DRV_DEVICE "/home/root/dev/spi_drv0"  // Adjust the device path based on your setup

int main() {
    int sw2_fd, spi_drv_fd;
    char sw2_buf[2];

    // Open the sw2 device
    sw2_fd = open(SW2_DEVICE, O_RDONLY);
    if (sw2_fd == -1) {
        perror("Error opening sw2 device");
        exit(EXIT_FAILURE);
    }

    // Open the spi_drv device
    spi_drv_fd = open(SPI_DRV_DEVICE, O_WRONLY);
    if (spi_drv_fd == -1) {
        perror("Error opening spi_drv device");
        close(sw2_fd);
        exit(EXIT_FAILURE);
    }

    // Read the button state from sw2
    if (read(sw2_fd, sw2_buf, sizeof(sw2_buf)) == -1) {
        perror("Error reading from sw2 device");
        close(sw2_fd);
        close(spi_drv_fd);
        exit(EXIT_FAILURE);
    }

    // Send the button state to spi_drv
    if (write(spi_drv_fd, sw2_buf, sizeof(sw2_buf)) == -1) {
        perror("Error writing to spi_drv device");
        close(sw2_fd);
        close(spi_drv_fd);
        exit(EXIT_FAILURE);
    }

    printf("Button state sent to spi_drv: %c\n", sw2_buf[0]);

    // Close file descriptors
    close(sw2_fd);
    close(spi_drv_fd);

    return EXIT_SUCCESS;
}
