#include <ad5940.h>
#include <zephyr/kernel.h>

#define SYSTICK_CLKFREQ   100000000L  /* Systick clock frequency in Hz. This only appies to NUCLEOF411 board */
volatile static uint8_t ucInterrupted = 0;       /* Flag to indicate interrupt occurred */

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

}

void AD5940_CsClr(void)
{
}

void AD5940_CsSet(void)
{
}

void AD5940_RstSet(void)
{
  
}

void AD5940_RstClr(void)
{
  
}

void AD5940_Delay10us(uint32_t time)
{

}

uint32_t AD5940_GetMCUIntFlag(void)
{
	return ucInterrupted;
}

uint32_t AD5940_ClrMCUIntFlag(void)
{
	ucInterrupted = 0;
	return 1;
}

uint32_t AD5940_MCUResourceInit(void *pCfg)
{
  return 0;
}

/* MCU related external line interrupt service routine */
void EXTI15_10_IRQHandler()
{
  ucInterrupted = 1;
}
