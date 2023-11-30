#include <ad5940.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>

#define _gpio(port, pin) DEVICE_DT_GET(DT_NODELABEL(gpio##port)), pin
#define CS_PIN _gpio(0, 24)
#define RST_PIN _gpio(1, 0)

#define _INT_GPIO_PORT DEVICE_DT_GET(DT_NODELABEL(gpio0))
#define _INT_GPIO_PIN 20
#define INT_PIN _gpio(0, _INT_GPIO_PIN)

const struct device *spi_dev = DEVICE_DT_GET(DT_NODELABEL(spi0));

K_MSGQ_DEFINE(run_int, 4, 30, 4);
void gpio_int_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    int dummy = 0;
    k_msgq_put(&run_int, &dummy, K_NO_WAIT);
}
static struct gpio_callback gpio_int_cb;


/**
    @brief Using SPI to transmit N bytes and return the received bytes. This function targets to
                        provide a more efficent way to transmit/receive data.
    @param pSendBuffer :{0 - 0xFFFFFFFF}
        - Pointer to the data to be sent.
    @param pRecvBuff :{0 - 0xFFFFFFFF}
        - Pointer to the buffer used to store received data.
    @param length :{0 - 0xFFFFFFFF}
        - Data length in SendBuffer.
    @return None.
**/
void AD5940_ReadWriteNBytes(unsigned char *pSendBuffer,unsigned char *pRecvBuff,unsigned long length)
{
    struct spi_config spi_cfg = {
        .frequency = 1000000,
        .operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB,
        .slave = 0,
        .cs = NULL,
    };

    struct spi_buf tx_buf = {
        .buf = pSendBuffer,
        .len = length
    };
    struct spi_buf_set tx = {
        .buffers = &tx_buf,
        .count = 1
    };

    struct spi_buf rx_buf = {
        .buf = pRecvBuff,
        .len = length
    };
    struct spi_buf_set rx = {
        .buffers = &rx_buf,
        .count = 1
    };

    spi_transceive(spi_dev, &spi_cfg, &tx, &rx);
}

void AD5940_CsClr(void)
{
    gpio_pin_set(CS_PIN, 0); // CS = Low
}

void AD5940_CsSet(void)
{
    gpio_pin_set(CS_PIN, 1); // CS = High
}

void AD5940_RstSet(void)
{
    gpio_pin_set(RST_PIN, 1); // RST = High
}

void AD5940_RstClr(void)
{
    gpio_pin_set(RST_PIN, 0); // RST = Low
}

void AD5940_Delay10us(uint32_t time)
{
    k_usleep(10*time);
}

uint32_t AD5940_GetMCUIntFlag(void)
{
    int dummy = 0;
    k_msgq_get(&run_int, &dummy, K_FOREVER);
    return 1;
}

uint32_t AD5940_ClrMCUIntFlag(void)
{
    return 1;
}

uint32_t AD5940_MCUResourceInit(void *pCfg)
{
    // set cs, rst
    gpio_pin_configure(CS_PIN, GPIO_OUTPUT);
    gpio_pin_configure(RST_PIN, GPIO_OUTPUT);

    //set interrupt
    gpio_pin_configure(INT_PIN, GPIO_INPUT);
    gpio_pin_interrupt_configure(INT_PIN, GPIO_INT_EDGE_FALLING);
    gpio_init_callback(&gpio_int_cb, gpio_int_callback, BIT(_INT_GPIO_PIN));
    gpio_add_callback(_INT_GPIO_PORT, &gpio_int_cb);
    return 0;
}
