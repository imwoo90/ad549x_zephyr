/*!
 *****************************************************************************
 @file:    AD5940Main.c
 @author:  $Author: nxu2 $
 @brief:   Used to control specific application and process data.
 @version: $Revision: 766 $
 @date:    $Date: 2017-08-21 14:09:35 +0100 (Mon, 21 Aug 2017) $
 -----------------------------------------------------------------------------

Copyright (c) 2017-2019 Analog Devices, Inc. All Rights Reserved.

This software is proprietary to Analog Devices, Inc. and its licensors.
By using this software you agree to the terms of the associated
Analog Devices Software License Agreement.
 
*****************************************************************************/
#include "ad5940.h"
#include <stdio.h>
#include "string.h"
#include "math.h"
#include "ChronoAmperometric.h"
#include <ui.h>
#include <ad5940_state.h>

#define INIT_VOLTAGE 1        /* 초기 전압 (V) */
#define HIGH_VOLTAGE -0.3      /* 인가 전압 (V) */
#define NUMBER_OF_STEP 1      /* 반복 횟수 */
#define PULSE_WIDTH 10        /* 전압 인가 시간 (s) */
#define SAMPLE_INTERVAL 0.05  /* 전류 측정 단위 (s) */
#define QUIET_TIME 2          /* 초기 대기 시간 (s) */

#define APPBUFF_SIZE 1000
#define n 3
#ifdef __ICCARM__
#pragma location="never_retained_ram"
#endif
uint32_t AppBuff[n][APPBUFF_SIZE];
float LFOSCFreq;
uint32_t IntCount = 0;
/* It's your choice here what to do with the data. Here is just an example to print to UART */
int32_t AMPShowResult(float *pData, uint32_t DataCount)
{
  /*static*/ uint32_t index = 0;
  /* Print data*/
	if(!IntCount)
		index = 0;
  for(int i=0;i<DataCount;i++)
  {
    printf("index:%d, %f\n", index++, pData[i]);
  }

  if (DataCount > 0) {
    set_ad5940_state(AD5940_STATE_READY);
    update_sensor_data(pData[DataCount/2], pData[DataCount-1]);
  }
  return 0;
}

/* Initialize AD5940 basic blocks like clock */
static int32_t AD5940PlatformCfg(void)
{
  CLKCfg_Type clk_cfg;
  FIFOCfg_Type fifo_cfg;
  AGPIOCfg_Type gpio_cfg;
	LFOSCMeasure_Type LfoscMeasure;

/* Use hardware reset */
  AD5940_HWReset();

  /* Platform configuration */
  AD5940_Initialize();
  /* Step1. Configure clock */
  clk_cfg.ADCClkDiv = ADCCLKDIV_1;
  clk_cfg.ADCCLkSrc = ADCCLKSRC_HFOSC;
  clk_cfg.SysClkDiv = SYSCLKDIV_1;
  clk_cfg.SysClkSrc = SYSCLKSRC_HFOSC;
  clk_cfg.HfOSC32MHzMode = bFALSE;
  clk_cfg.HFOSCEn = bTRUE;
  clk_cfg.HFXTALEn = bFALSE;
  clk_cfg.LFOSCEn = bTRUE;
  AD5940_CLKCfg(&clk_cfg);
  /* Step2. Configure FIFO and Sequencer*/
  fifo_cfg.FIFOEn = bFALSE;
  fifo_cfg.FIFOMode = FIFOMODE_FIFO;
  fifo_cfg.FIFOSize = FIFOSIZE_4KB;                       /* 4kB for FIFO, The reset 2kB for sequencer */
  fifo_cfg.FIFOSrc = FIFOSRC_DFT;
  fifo_cfg.FIFOThresh = 1024;//AppAMPCfg.FifoThresh;        /* DFT result. One pair for RCAL, another for Rz. One DFT result have real part and imaginary part */
  AD5940_FIFOCfg(&fifo_cfg);                             /* Disable to reset FIFO. */
	fifo_cfg.FIFOEn = bTRUE;  
  AD5940_FIFOCfg(&fifo_cfg);                             /* Enable FIFO here */
  
  /* Step3. Interrupt controller */
  AD5940_INTCCfg(AFEINTC_1, AFEINTSRC_ALLINT, bTRUE);           /* Enable all interrupt in Interrupt Controller 1, so we can check INTC flags */
  AD5940_INTCClrFlag(AFEINTSRC_ALLINT);
  AD5940_INTCCfg(AFEINTC_0, AFEINTSRC_DATAFIFOTHRESH|AFEINTSRC_ENDSEQ, bTRUE);   /* Interrupt Controller 0 will control GP0 to generate interrupt to MCU */
  AD5940_INTCClrFlag(AFEINTSRC_ALLINT);
  /* Step4: Reconfigure GPIO */
  gpio_cfg.FuncSet = GP6_SYNC|GP5_SYNC|GP4_SYNC|GP2_TRIG|GP1_SYNC|GP0_INT;
  gpio_cfg.InputEnSet = AGPIO_Pin2;
  gpio_cfg.OutputEnSet = AGPIO_Pin0|AGPIO_Pin1|AGPIO_Pin4|AGPIO_Pin5|AGPIO_Pin6;
  gpio_cfg.OutVal = 0;
  gpio_cfg.PullEnSet = 0;
  AD5940_AGPIOCfg(&gpio_cfg);
	
	AD5940_SleepKeyCtrlS(SLPKEY_UNLOCK);  /* Enable AFE to enter sleep mode. */
  /* Measure LFOSC frequency */
  LfoscMeasure.CalDuration = QUIET_TIME*1000.0;  /* 1000ms used for calibration. */
  LfoscMeasure.CalSeqAddr = 0;
  LfoscMeasure.SystemClkFreq = 16000000.0f; /* 16MHz in this firmware. */
  AD5940_LFOSCMeasure(&LfoscMeasure, &LFOSCFreq);
  printf("Freq:%f\n", LFOSCFreq); 
	
  return 0;
}

/* !!Change the application parameters here if you want to change it to none-default value */
void AD5940AMPStructInit(void)
{
  AppCHRONOAMPCfg_Type *pAMPCfg; 
  AppCHRONOAMPGetCfg(&pAMPCfg);
  /* Configure general parameters */
	pAMPCfg->WuptClkFreq = LFOSCFreq;					/* Use measured 32kHz clock freq for accurate wake up timer */
  pAMPCfg->SeqStartAddr = 0;
  pAMPCfg->MaxSeqLen = 512; 								/* @todo add checker in function */
  pAMPCfg->RcalVal = 10000.0;
  pAMPCfg->NumOfData = NUMBER_OF_STEP;      						/* Never stop until you stop it manually by AppAMPCtrl() function */
	
	pAMPCfg->AmpODR = 1;
	pAMPCfg->FifoThresh = PULSE_WIDTH / SAMPLE_INTERVAL;
	pAMPCfg->ADCRefVolt = 1.82;							/* Measure voltage on VREF_1V8 pin and add here */
	
	pAMPCfg->ExtRtia = bFALSE;			/* Set to true if using external Rtia */
	pAMPCfg->ExtRtiaVal = 10000000; /* Enter external Rtia value here is using one */
	pAMPCfg->LptiaRtiaSel = LPTIARTIA_1K;		/* Select TIA gain resistor. */
	
	pAMPCfg->SensorBias = 0;   /* Vre - Vse : Sensor bias voltage between reference and sense electrodes*/
	pAMPCfg->Vzero = INIT_VOLTAGE*1000;
  pAMPCfg->Vbias = 0;
	/* Configure Pulse*/
  pAMPCfg->pulseAmplitude = HIGH_VOLTAGE*1000;						/* Pulse amplitude on counter electrode (mV) */
	pAMPCfg->pulseLength = PULSE_WIDTH*1000;								/* Length of voltage pulse in ms */
	
  /* Vre Vse Vce : Hardware Electrode Information*/
}

void AD5940_Main(void)
{
  uint32_t temp[n];
  AppCHRONOAMPCfg_Type *pAMPCfg;
  AppCHRONOAMPGetCfg(&pAMPCfg);
  AD5940PlatformCfg();
 
  set_ad5940_state(AD5940_STATE_READY);

  while(1)
  {
    /* Check if interrupt flag which will be set when interrupt occurred. */
    if(AD5940_GetMCUIntFlag())
    {
      AD5940_ClrMCUIntFlag(); /* Clear this flag */
      printf("here5\n");
      temp[IntCount] = APPBUFF_SIZE;
      AppCHRONOAMPISR(AppBuff[IntCount], &temp[IntCount]); /* Deal with it and provide a buffer to store data we got */
			
      /* 기본 측정 시퀀스 */
      if(pAMPCfg->bMeasureTransient == bFALSE)
			{
				AMPShowResult((float*)AppBuff[0], temp[0]);
			}
      printf("here6\n");

      /* 기본 시퀀스 종료 후 다시 측정 하는 부분 */
      if(pAMPCfg->EndSeq) /* End sequence only set at end of transient */
      {
        printf("here7\n");
        for(int i = 0; i<IntCount; i++)
        {
          AMPShowResult((float*)AppBuff[i], temp[i]); /* Show the results to UART */
        }
        printf("here9\n");
        pAMPCfg->EndSeq = bFALSE;               /* !bFALSE일 경우 무한 반복 측정 */
				pAMPCfg->bMeasureTransient = bFALSE;
        IntCount = 0;
				AppCHRONOAMPCtrl(CHRONOAMPCTRL_START, 0); /* Begin standard amperometric measurement after pulse test is complete */
        printf("here10\n");
      }
    }
  }
}

