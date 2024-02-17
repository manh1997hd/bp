/**
 **************************************************************************************
 * @file    Spdif_mode.c
 * @brief
 *
 * @author  Sam
 * @version V1.0.0
 *
 * $Created: 2017-12-26 13:06:47$
 *
 * @Copyright (C) 2016, Shanghai Mountain View Silicon Co.,Ltd. All rights reserved.
 **************************************************************************************
 */
 
#include <string.h>
#include "type.h"
#include "irqn.h"
#include "gpio.h"
#include "dma.h"
#include "rtos_api.h"
#include "app_message.h"
#include "app_config.h"
#include "debug.h"
#include "delay.h"
#include "audio_adc.h"
#include "dac.h"
#include "adc_interface.h"
#include "spdif.h"
#include "dac_interface.h"
#include "clk.h"
#include "audio_core_api.h"
#include "audio_core_service.h"
#include "audio_effect.h"
#include "powercontroller.h"
#include "deepsleep.h"
#include "backup_interface.h"
#include "resampler_polyphase.h"
#include "mcu_circular_buf.h"
#include "breakpoint.h"
#include "otg_device_hcd.h"
#include "otg_device_standard_request.h"
#include "main_task.h"
#include "remind_sound_item.h"
#include "mode_task_api.h"
#include "remind_sound.h"
#include "spdif_mode.h"
#include "audio_vol.h"
#include "ctrlvars.h"
#include "reset.h"
#include "audio_effect_flash_param.h"

#if defined(CFG_APP_OPTICAL_MODE_EN) || defined(CFG_APP_COAXIAL_MODE_EN)

bool SpdifLockFlag = FALSE;
uint32_t samplerate = 0;

#define SPDIF_SOURCE_NUM				APP_SOURCE_NUM
#define PCM_REMAIN_SMAPLES				3//spdif���ݶ�����ʱ,�������ݻᳬ��һ֡128�����ڻ������ݣ������и��Ӻ�
//spdif����������8�ֽ�
//recv, dma buf len,MAX_FRAME_SAMPLES * 2 * 2 * 2 * 2�ǻ�����OS�л����ʵ����Ҫ�ӱ���
#if (defined(CFG_AUDIO_WIDTH_24BIT) && !defined(BT_TWS_SUPPORT) )
#define	SPDIF_FIFO_LEN					(MAX_FRAME_SAMPLES * 2 * 2 * 2 * 2 * 2 * 2)
#else
#define	SPDIF_FIFO_LEN					(MAX_FRAME_SAMPLES * 2 * 2 * 2 * 2 * 2)
#endif

//���ڲ���ֵ��32bitת��Ϊ16bit������ʹ��ͬһ��buf������Ҫ��������
#if (defined(CFG_AUDIO_WIDTH_24BIT) && !defined(BT_TWS_SUPPORT) )
#define SPDIF_CARRY_LEN					(MAX_FRAME_SAMPLES * 2 * 2 * 2 * 2 + PCM_REMAIN_SMAPLES * 2 * 4)//֧��192000���� buf len for get data form dma fifo, deal + 6samples ���ڻ���ż����������ݣ�#9817
#else
#define SPDIF_CARRY_LEN					(MAX_FRAME_SAMPLES * 2 * 2 * 2 + PCM_REMAIN_SMAPLES * 2 * 4)
#endif
//ת�������buf,���spdifת�������������ı���Ҫ�Ӵ��SPDIF_CARRY_LEN����������8000����ת48000,��Ҫ��С����carry֡��С�����SPDIF_RESAMPLER_OUT_LEN��SPDIF_PCM_FIFO_LEN
#define SPDIF_RESAMPLER_OUT_LEN			(MAX_FRAME_SAMPLES * 2 * 2 * 4)


/**����appconfigȱʡ����:DMA 8��ͨ������**/
/*1��cec��PERIPHERAL_ID_TIMER3*/
/*2��SD��¼����PERIPHERAL_ID_SDIO RX/TX*/
/*3�����ߴ��ڵ�����PERIPHERAL_ID_UART1 RX/TX,����ʹ��USB HID����ʡDMA��Դ*/
/*4����·������PERIPHERAL_ID_AUDIO_ADC0_RX*/
/*5��Mic������PERIPHERAL_ID_AUDIO_ADC1_RX��mode֮��ͨ������һ��*/
/*6��Dac0������PERIPHERAL_ID_AUDIO_DAC0_TX mode֮��ͨ������һ��*/
/*7��DacX�迪��PERIPHERAL_ID_AUDIO_DAC1_TX mode֮��ͨ������һ��*/
/*ע��DMA 8��ͨ�����ó�ͻ:*/
/*a��UART���ߵ�����DAC-X�г�ͻ,Ĭ�����ߵ���ʹ��USB HID*/
/*b��UART���ߵ�����HDMI/SPDIFģʽ��ͻ*/
/*ע��DMA 8��ͨ�����ó�ͻ:*/
/*a��UART���ߵ�����DAC-X�г�ͻ,Ĭ�����ߵ���ʹ��USB HID*/
/*b��UART���ߵ�����HDMI/SPDIFģʽ��ͻ*/
static const uint8_t DmaChannelMap[29] =
{
	255,//PERIPHERAL_ID_SPIS_RX = 0,	//0
	255,//PERIPHERAL_ID_SPIS_TX,		//1

#ifdef CFG_APP_HDMIIN_MODE_EN
	5,//PERIPHERAL_ID_TIMER3,			//2
#else
	255,//PERIPHERAL_ID_TIMER3,			//2
#endif

#if defined (CFG_FUNC_I2S_MIX_MODE) && defined (CFG_RES_AUDIO_I2S1IN_EN)
	255,//PERIPHERAL_ID_SDIO_RX,		//3
	255,//PERIPHERAL_ID_SDIO_TX,		//4
#else
	4,//PERIPHERAL_ID_SDIO_RX,			//3
	4,//PERIPHERAL_ID_SDIO_TX,			//4
#endif

	255,//PERIPHERAL_ID_UART0_RX,		//5
	255,//PERIPHERAL_ID_TIMER1,			//6
	255,//PERIPHERAL_ID_TIMER2,			//7
	0,//PERIPHERAL_ID_SDPIF_RX,			//8 SPDIF_RX /TX same chanell
	0,//PERIPHERAL_ID_SDPIF_TX,			//8 SPDIF_RX /TX same chanell
	255,//PERIPHERAL_ID_SPIM_RX,		//9
	255,//PERIPHERAL_ID_SPIM_TX,		//10
	255,//PERIPHERAL_ID_UART0_TX,		//11
	
#ifdef CFG_COMMUNICATION_BY_UART	
	7,//PERIPHERAL_ID_UART1_RX,			//12
	6,//PERIPHERAL_ID_UART1_TX,			//13
#else
	255,//PERIPHERAL_ID_UART1_RX,		//12
	255,//PERIPHERAL_ID_UART1_TX,		//13
#endif

	255,//PERIPHERAL_ID_TIMER4,			//14
	255,//PERIPHERAL_ID_TIMER5,			//15
	255,//PERIPHERAL_ID_TIMER6,			//16
	255,//PERIPHERAL_ID_AUDIO_ADC0_RX,	//17
	1,//PERIPHERAL_ID_AUDIO_ADC1_RX,	//18
	2,//PERIPHERAL_ID_AUDIO_DAC0_TX,	//19
	3,//PERIPHERAL_ID_AUDIO_DAC1_TX,	//20

#if defined (CFG_FUNC_I2S_MIX_MODE) && defined (CFG_RES_AUDIO_I2S0IN_EN)	
	6,//PERIPHERAL_ID_I2S0_RX,		//21
#else
	255,//PERIPHERAL_ID_I2S0_RX,		//21
#endif

#if	((defined(CFG_RES_AUDIO_I2SOUT_EN )&&(CFG_RES_I2S == 0)) || defined(CFG_RES_AUDIO_I2S0OUT_EN))
	7,//PERIPHERAL_ID_I2S0_TX,		//22
#else	
	255,//PERIPHERAL_ID_I2S0_TX,		//22
#endif	

#if defined (CFG_FUNC_I2S_MIX_MODE) && defined (CFG_RES_AUDIO_I2S1IN_EN)	
	4,//PERIPHERAL_ID_I2S1_RX,		//23
#else
	255,//PERIPHERAL_ID_I2S1_RX,		//23
#endif

#if	((defined(CFG_RES_AUDIO_I2SOUT_EN )&&(CFG_RES_I2S == 1))|| defined(CFG_RES_AUDIO_I2S1OUT_EN))
	5,	//PERIPHERAL_ID_I2S1_TX,		//24
#else
	255,//PERIPHERAL_ID_I2S1_TX,		//24
#endif

	255,//PERIPHERAL_ID_PPWM,			//25
	255,//PERIPHERAL_ID_ADC,     		//26
	255,//PERIPHERAL_ID_SOFTWARE,		//27
};

typedef struct _SpdifPlayContext
{
	//xTaskHandle 		taskHandle;
	//MessageHandle		msgHandle;
	//MessageHandle		parentMsgHandle;

	TaskState			state;
	uint32_t			*SpdifPwcFIFO;		//
	uint16_t 			*Source1Buf_Spdif;//
	uint32_t            *SpdifCarry;
#if (defined(CFG_AUDIO_WIDTH_24BIT) && !defined(BT_TWS_SUPPORT) )
	uint32_t            *SpdifCarry24;
#endif
	uint32_t			*SpdifPcmFifo;
	MCU_CIRCULAR_CONTEXT SpdifPcmCircularBuf;

	AudioCoreContext 	*AudioCoreSpdif;
#if	defined(CFG_FUNC_REMIND_SOUND_EN)
	uint16_t*			Source2Decoder;
	TaskState			DecoderSync;
	bool				IsSoundRemindDone;
#endif

#ifdef CFG_FUNC_RECORDER_EN
	TaskState			RecorderSync;
#endif
	//play
	uint32_t 			SampleRate;
	
#if 1//def CFG_FUNC_MIXER_SRC_EN
	ResamplerPolyphaseContext* ResamplerCt;
	uint32_t*			resampleOutBuf;
#endif
	uint32_t			SpdifSampleRate;

	uint32_t 			SpdifDmaWritePtr;
	uint32_t 			SpdifDmaReadPtr;
	uint32_t 			SpdifPreSample;
	uint8_t  			SpdifSampleRateCheckFlg;
	uint32_t 			SpdifSampleRateFromSW;

}SpdifPlayContext;
static  SpdifPlayContext*		SpdifPlayCt = NULL;

static const int32_t SamplerRatio[][3] =
{
	{8000,  RESAMPLER_POLYPHASE_SRC_RATIO_441_80,	RESAMPLER_POLYPHASE_SRC_RATIO_6_1},
	{11025, RESAMPLER_POLYPHASE_SRC_RATIO_4_1,		RESAMPLER_POLYPHASE_SRC_RATIO_640_147},
	{12000, RESAMPLER_POLYPHASE_SRC_RATIO_147_40,	RESAMPLER_POLYPHASE_SRC_RATIO_4_1},
	{16000, RESAMPLER_POLYPHASE_SRC_RATIO_441_160,	RESAMPLER_POLYPHASE_SRC_RATIO_3_1},
	{22050, RESAMPLER_POLYPHASE_SRC_RATIO_2_1,		RESAMPLER_POLYPHASE_SRC_RATIO_320_147},
	{24000, RESAMPLER_POLYPHASE_SRC_RATIO_147_80,	RESAMPLER_POLYPHASE_SRC_RATIO_2_1},
	{32000, RESAMPLER_POLYPHASE_SRC_RATIO_441_320, 	RESAMPLER_POLYPHASE_SRC_RATIO_3_2},
	{44100, 0,										RESAMPLER_POLYPHASE_SRC_RATIO_160_147},
	{48000, RESAMPLER_POLYPHASE_SRC_RATIO_147_160,	0},
	{88200, RESAMPLER_POLYPHASE_SRC_RATIO_1_2,		RESAMPLER_POLYPHASE_SRC_RATIO_80_147},
	{96000, RESAMPLER_POLYPHASE_SRC_RATIO_147_320,	RESAMPLER_POLYPHASE_SRC_RATIO_1_2},
	{176400,RESAMPLER_POLYPHASE_SRC_RATIO_1_4,		RESAMPLER_POLYPHASE_SRC_RATIO_40_147},
	{192000,RESAMPLER_POLYPHASE_SRC_RATIO_147_640,	RESAMPLER_POLYPHASE_SRC_RATIO_1_4},
	{33075, RESAMPLER_POLYPHASE_SRC_RATIO_4_3,		RESAMPLER_POLYPHASE_SRC_RATIO_4_3},
};

int32_t Get_Resampler_Polyphase(uint32_t resampler)
{
	int32_t res = 0;
	uint32_t i;
	for(i=0; i<14; i++)
	{
		if(SamplerRatio[i][0] == resampler)
		{
			if(CFG_PARA_SAMPLE_RATE == 44100)
			{
				res = SamplerRatio[i][1];
				break;
			}
			else if(CFG_PARA_SAMPLE_RATE == 48000)
			{
				res = SamplerRatio[i][2];
				break;
			}
			else
			{
				res = 0;
			}
		}
	}
	//APP_DBG("res = %d\n", res);		
	return res;
}

uint16_t Spdif_Rx_DataLenGet(void);
uint16_t Spdif_Rx_DataGet(void *pcm_out, uint16_t MaxSize);
void SpdifDataCarry(void);
static void SpdifPlayRunning(uint16_t msgId);
static void SpdifSampleRateChange(void)
{
#if 1//def CFG_FUNC_MIXER_SRC_EN
	resampler_polyphase_init(SpdifPlayCt->ResamplerCt, 2, Get_Resampler_Polyphase(SpdifPlayCt->SpdifSampleRate));
#else
	if(SpdifPlayCt->SampleRate != SpdifPlayCt->SpdifSampleRate)
	{
		SpdifPlayCt->SampleRate = SpdifPlayCt->SpdifSampleRate;//ע��˴������������ʾ����mic����dac��������
		APP_DBG("Dac Sample:%d\n",(int)SpdifPlayCt->SampleRate);
#ifdef CFG_RES_AUDIO_DACX_EN
		AudioDAC_SampleRateChange(ALL, SpdifPlayCt->SampleRate);
#endif
#ifdef CFG_RES_AUDIO_DAC0_EN
		AudioDAC_SampleRateChange(DAC0, SpdifPlayCt->SampleRate);
#endif
	}
#endif
}
static void SpdifPlayRunning(uint16_t msgId)
{
	switch(msgId)
	{
	
		default:
			CommonMsgProccess(msgId);
			break;
	}
}

//sampleΪ��λ
uint16_t Spdif_Rx_DataLenGet(void)
{
#if (defined(CFG_AUDIO_WIDTH_24BIT) && !defined(BT_TWS_SUPPORT) )
	return MCUCircular_GetDataLen(&SpdifPlayCt->SpdifPcmCircularBuf)/8;
#else
	return MCUCircular_GetDataLen(&SpdifPlayCt->SpdifPcmCircularBuf)/4;
#endif
}

//sampleΪ��λ��buf��С��8 * MaxSize
uint16_t Spdif_Rx_DataGet(void *pcm_out, uint16_t MaxPoint)
{
#if (defined(CFG_AUDIO_WIDTH_24BIT) && !defined(BT_TWS_SUPPORT) )
	return MCUCircular_GetData(&SpdifPlayCt->SpdifPcmCircularBuf, pcm_out, MaxPoint * 8) / 8;
#else
	return MCUCircular_GetData(&SpdifPlayCt->SpdifPcmCircularBuf, pcm_out, MaxPoint * 4) / 4;
#endif
}

void SpdifDataCarry(void)
{

#if (defined(CFG_AUDIO_WIDTH_24BIT) && !defined(BT_TWS_SUPPORT) )
	int16_t pcm_space;
	uint16_t spdif_len;
	int16_t pcm_len;
	uint32_t *pcmBuf  = (int16_t *)SpdifPlayCt->SpdifCarry;
	int16_t *pcmBuf24  = (int16_t *)SpdifPlayCt->SpdifCarry24;
	uint16_t cnt;

	spdif_len = DMA_CircularDataLenGet(PERIPHERAL_ID_SPDIF_RX);
	pcm_space = MCUCircular_GetSpaceLen(&SpdifPlayCt->SpdifPcmCircularBuf) - 32;

	if(pcm_space < 32)
	{
		DBG("err\n");
		return;
	}
#if 1//def CFG_FUNC_MIXER_SRC_EN
	pcm_space = (pcm_space * SpdifPlayCt->SpdifSampleRate) / CFG_PARA_SAMPLE_RATE - 32;
#endif

	if((spdif_len >> 1) > pcm_space)
	{
		spdif_len = pcm_space * 2;
	}

	spdif_len = spdif_len & 0xFFF8;
	if(!spdif_len)
	{
		return ;
	}

	cnt = (spdif_len / 16) / (MAX_FRAME_SAMPLES);
	while(cnt--)
	{
		pcm_len = DMA_CircularDataGet(PERIPHERAL_ID_SPDIF_RX, pcmBuf, MAX_FRAME_SAMPLES * 16);

		//���ڴ�32bitת��Ϊ16bit��buf����ʹ��ͬһ��������Ҫ�������롣
		pcm_len = SPDIF_SPDIFDataToPCMData((int32_t *)pcmBuf, MAX_FRAME_SAMPLES * 16, (int32_t *)pcmBuf24, SPDIF_WORDLTH_24BIT);
		//printf("pcm_len = %d\n",pcm_len);
		if(pcm_len < 0)
		{
			return;
		}

#if 1//def CFG_FUNC_MIXER_SRC_EN
		if(SpdifPlayCt->SpdifSampleRate != CFG_PARA_SAMPLE_RATE)
		{
			pcm_len = resampler_polyphase_apply(SpdifPlayCt->ResamplerCt, (int16_t*)pcmBuf24, (int16_t*)SpdifPlayCt->resampleOutBuf, pcm_len/4);
			if(pcm_len<0)
			{
				return ;
			}
			MCUCircular_PutData(&SpdifPlayCt->SpdifPcmCircularBuf, SpdifPlayCt->resampleOutBuf, pcm_len*4);
		}
		else
		{
			MCUCircular_PutData(&SpdifPlayCt->SpdifPcmCircularBuf, pcmBuf24, pcm_len);
		}
#else
			MCUCircular_PutData(&SpdifPlayCt->SpdifPcmCircularBuf, pcmBuf24, pcm_len);
#endif
	}

#else

	int16_t pcm_space;
	uint16_t spdif_len;
	int16_t pcm_len;
	uint32_t *pcmBuf = SpdifPlayCt->SpdifCarry;
	uint16_t cnt;

	spdif_len = DMA_CircularDataLenGet(PERIPHERAL_ID_SPDIF_RX);
	pcm_space = MCUCircular_GetSpaceLen(&SpdifPlayCt->SpdifPcmCircularBuf) - 16;

	if(pcm_space < 16)
	{
		DBG("err\n");
		return;
	}
#if 1//def CFG_FUNC_MIXER_SRC_EN
	pcm_space = (pcm_space * SpdifPlayCt->SpdifSampleRate) / CFG_PARA_SAMPLE_RATE - 16;
#endif

	if((spdif_len >> 1) > pcm_space)
	{
		spdif_len = pcm_space * 2;
	}

	spdif_len = spdif_len & 0xFFF8;
	if(!spdif_len)
	{
		return ;
	}

	cnt = (spdif_len / 8) / (MAX_FRAME_SAMPLES);
	while(cnt--)
	{
		pcm_len = DMA_CircularDataGet(PERIPHERAL_ID_SPDIF_RX, pcmBuf, MAX_FRAME_SAMPLES * 8);

		//���ڴ�32bitת��Ϊ16bit��buf����ʹ��ͬһ��������Ҫ�������롣
		pcm_len = SPDIF_SPDIFDataToPCMData((int32_t *)pcmBuf, MAX_FRAME_SAMPLES * 8, (int32_t *)pcmBuf, SPDIF_WORDLTH_16BIT);
		//printf("pcm_len = %d\n",pcm_len);
		if(pcm_len < 0)
		{
			return;
		}

#if 1//def CFG_FUNC_MIXER_SRC_EN
		if(SpdifPlayCt->SpdifSampleRate != CFG_PARA_SAMPLE_RATE)
		{
			pcm_len = resampler_polyphase_apply(SpdifPlayCt->ResamplerCt, (int16_t*)pcmBuf, (int16_t*)SpdifPlayCt->resampleOutBuf, pcm_len/4);
			if(pcm_len<0)
			{
				return ;
			}
			MCUCircular_PutData(&SpdifPlayCt->SpdifPcmCircularBuf, SpdifPlayCt->resampleOutBuf, pcm_len*4);
		}
		else
		{
			MCUCircular_PutData(&SpdifPlayCt->SpdifPcmCircularBuf, pcmBuf, pcm_len);
		}
#else
			MCUCircular_PutData(&SpdifPlayCt->SpdifPcmCircularBuf, pcmBuf, pcm_len);
#endif
	}
#endif
}


bool SpdifPlayInit(void)
{
	
	AudioCoreIO AudioIOSet;
	bool ret;

	
	if(SpdifPlayCt != NULL)
	{
		return FALSE;
	}	
	
#ifdef CFG_FUNC_REMIND_SOUND_EN
	{
		if(GetSystemMode() == ModeOpticalAudioPlay)
		{
			DBG("Optical Play Init\n");
		}
		else
		{
			DBG("Coaxial Play Init\n");
		}
	}		
#endif

	SPDIF_ModuleDisable();
	Clock_SpdifClkSelect(APLL_CLK_MODE);
	DMA_ChannelAllocTableSet((uint8_t*)DmaChannelMap);//optical
	
	SpdifPlayCt = (SpdifPlayContext*)osPortMalloc(sizeof(SpdifPlayContext));
	if(SpdifPlayCt == NULL)
	{
		return FALSE;
	}
	memset(SpdifPlayCt, 0, sizeof(SpdifPlayContext));

	SpdifPlayCt->SampleRate = 0;//CFG_PARA_SAMPLE_RATE;
	
	SpdifPlayCt->SpdifPcmFifo = (uint32_t *)osPortMalloc(AudioCoreFrameSizeGet(DefaultNet) * 2 * 2 * 2 * 2 * 2);//end bkd
	if(SpdifPlayCt->SpdifPcmFifo == NULL)
	{
		return FALSE;
	}
	memset(SpdifPlayCt->SpdifPcmFifo, 0, AudioCoreFrameSizeGet(DefaultNet) * 2 * 2 * 2 * 2 * 2);
	MCUCircular_Config(&SpdifPlayCt->SpdifPcmCircularBuf, SpdifPlayCt->SpdifPcmFifo, AudioCoreFrameSizeGet(DefaultNet) * 2 * 2 * 2 * 2 * 2);
	DBG("spdif frame size %d sample / frame\n",AudioCoreFrameSizeGet(DefaultNet));


	//  (DMA buffer)
	SpdifPlayCt->SpdifPwcFIFO = (uint32_t*)osPortMalloc(SPDIF_FIFO_LEN);
	if(SpdifPlayCt->SpdifPwcFIFO == NULL)
	{
		return FALSE;
	}
	memset(SpdifPlayCt->SpdifPwcFIFO, 0, SPDIF_FIFO_LEN);
	

	SpdifPlayCt->SpdifCarry = (uint32_t *)osPortMalloc(SPDIF_CARRY_LEN);
	if(SpdifPlayCt->SpdifCarry == NULL)
	{
		return FALSE;
	}
	memset(SpdifPlayCt->SpdifCarry, 0, SPDIF_CARRY_LEN);

#if (defined(CFG_AUDIO_WIDTH_24BIT) && !defined(BT_TWS_SUPPORT) )
	SpdifPlayCt->SpdifCarry24 = (uint32_t *)osPortMalloc(SPDIF_CARRY_LEN);
		if(SpdifPlayCt->SpdifCarry24 == NULL)
		{
			return FALSE;
		}
		memset(SpdifPlayCt->SpdifCarry24, 0, SPDIF_CARRY_LEN);
#endif

#if 1//def CFG_FUNC_MIXER_SRC_EN
	SpdifPlayCt->ResamplerCt = (ResamplerPolyphaseContext*)osPortMalloc(sizeof(ResamplerPolyphaseContext));
	if(SpdifPlayCt->ResamplerCt == NULL)
	{
		return FALSE;
	}
	memset(SpdifPlayCt->ResamplerCt, 0, sizeof(ResamplerPolyphaseContext));

	SpdifPlayCt->resampleOutBuf = (uint32_t *)osPortMalloc(SPDIF_RESAMPLER_OUT_LEN);
	if(SpdifPlayCt->resampleOutBuf == NULL)
	{
		return FALSE;
	}
	memset(SpdifPlayCt->resampleOutBuf, 0, SPDIF_RESAMPLER_OUT_LEN);
#endif

	SpdifPlayCt->SpdifDmaWritePtr 		 = 0;
	SpdifPlayCt->SpdifDmaReadPtr  		 = 0;
	SpdifPlayCt->SpdifSampleRateCheckFlg = 0;
	SpdifPlayCt->SpdifSampleRateFromSW 	 = 0;
	SpdifPlayCt->SpdifPreSample			 = 0;

	if(!ModeCommonInit())
	{
		return FALSE;
	}

	SpdifPlayCt->AudioCoreSpdif = (AudioCoreContext*)&AudioCore;

#ifdef CFG_FUNC_AUDIO_EFFECT_EN
#ifdef CFG_FUNC_MIC_KARAOKE_EN
	SpdifPlayCt->AudioCoreSpdif->AudioEffectProcess = (AudioCoreProcessFunc)AudioEffectProcess;
#else
	SpdifPlayCt->AudioCoreSpdif->AudioEffectProcess = (AudioCoreProcessFunc)AudioMusicProcess;
#endif
#else
	SpdifPlayCt->AudioCoreSpdif->AudioEffectProcess = (AudioCoreProcessFunc)AudioBypassProcess;
#endif

	//Audio init
	
	memset(&AudioIOSet, 0, sizeof(AudioCoreIO));

	AudioIOSet.Adapt = STD;//STD;
	AudioIOSet.Sync = FALSE;
	AudioIOSet.Channels = 2;
	AudioIOSet.Net = DefaultNet;

	AudioIOSet.DataIOFunc = Spdif_Rx_DataGet;
	AudioIOSet.LenGetFunc = Spdif_Rx_DataLenGet;
	
	AudioIOSet.Depth = AudioCoreFrameSizeGet(DefaultNet) * 2;
	AudioIOSet.LowLevelCent = 40;
	AudioIOSet.HighLevelCent = 60;
#ifdef	CFG_AUDIO_WIDTH_24BIT
#ifdef BT_TWS_SUPPORT
	AudioIOSet.IOBitWidth = 0;//0,16bit,1:24bit
	AudioIOSet.IOBitWidthConvFlag = 0;//tws����Ҫ���ݽ���λ����չ������TWS_SOURCE_NUM�Ժ�ͳһת��24bit
#else
	AudioIOSet.IOBitWidth = 1;//0,16bit,1:24bit
	AudioIOSet.IOBitWidthConvFlag = 0;//��Ҫ���ݽ���λ����չ
#endif
#endif
	if(!AudioCoreSourceInit(&AudioIOSet, SPDIF_SOURCE_NUM))
	{
		DBG("spdif source init error!\n");
		return FALSE;
	}
	
	AudioCoreSourceAdjust(SPDIF_SOURCE_NUM, TRUE);

#ifdef CFG_APP_OPTICAL_MODE_EN
	if(GetSystemMode() == ModeOpticalAudioPlay)
	{
		//spdif config
#ifndef PORT_B_INPUT_DIGATAL	//bkd add	
		GPIO_PortAModeSet(SPDIF_OPTICAL_INDEX, SPDIF_OPTICAL_PORT_MODE);
#else
		GPIO_PortBModeSet(SPDIF_OPTICAL_INDEX, SPDIF_OPTICAL_PORT_MODE);
#endif
#ifdef CFG_APP_COAXIAL_MODE_EN
		GPIO_PortAModeSet(SPDIF_COAXIAL_INDEX, 0);
#endif
		//SPDIF_AnalogModuleDisable();//bkd mark
		SPDIF_AnalogModuleEnable(SPDIF_OPTICAL_PORT_ANA_INPUT, SPDIF_ANA_LEVEL_300mVpp);
	}
#endif

#ifdef CFG_APP_COAXIAL_MODE_EN
	if(GetSystemMode() == ModeCoaxialAudioPlay)
	{	
		GPIO_PortAModeSet(SPDIF_COAXIAL_INDEX, SPDIF_COAXIAL_PORT_MODE);
#ifdef CFG_APP_OPTICAL_MODE_EN
		GPIO_PortAModeSet(SPDIF_OPTICAL_INDEX, 0);
#endif
		SPDIF_AnalogModuleEnable(SPDIF_COAXIAL_PORT_ANA_INPUT, SPDIF_ANA_LEVEL_300mVpp);
	}
#endif


	DMA_ChannelDisable(PERIPHERAL_ID_SPDIF_RX);
	//Reset_FunctionReset(DMAC_FUNC_SEPA);
	SPDIF_RXInit(1, 0, 0);
	//ʹ��
	DMA_CircularConfig(PERIPHERAL_ID_SPDIF_RX, SPDIF_FIFO_LEN / 2, (void*)SpdifPlayCt->SpdifPwcFIFO, SPDIF_FIFO_LEN);
	DMA_ChannelEnable(PERIPHERAL_ID_SPDIF_RX);
	SPDIF_ModuleEnable();

	
#ifdef CFG_FUNC_BREAKPOINT_EN
		BackupInfoUpdata(BACKUP_SYS_INFO);
#endif


#ifdef CFG_FUNC_AUDIO_EFFECT_EN
#ifdef CFG_EFFECT_PARAM_IN_FLASH_EN
	//mainAppCt.EffectMode = EFFECT_MODE_FLASH_Music;
#else
	mainAppCt.EffectMode = EFFECT_MODE_NORMAL;
#endif
	AudioEffectsLoadInit(1, mainAppCt.EffectMode);
	AudioAPPDigitalGianProcess(mainAppCt.SysCurrentMode);
	AudioEffectsLoadInit(0, mainAppCt.EffectMode);
#endif

#ifdef CFG_FUNC_REMIND_SOUND_EN
	if(GetSystemMode() == ModeOpticalAudioPlay)
	{
		DBG("Optical Play run\n");
		ret = RemindSoundServiceItemRequest(SOUND_REMIND_GXIANMOD, REMIND_PRIO_NORMAL);
	}
	else
	{
		DBG("Coaxial Play run\n");
		ret = RemindSoundServiceItemRequest(SOUND_REMIND_TZHOUMOD, REMIND_PRIO_NORMAL);
	}
	if(ret == FALSE)
	{
		if(IsAudioPlayerMute() == TRUE)
		{
			HardWareMuteOrUnMute();
		}
	}
#endif

	SpdifLockFlag = FALSE;
	samplerate = 0;
	
#ifndef CFG_FUNC_REMIND_SOUND_EN
	 if(IsAudioPlayerMute() == TRUE)
	 {
		 HardWareMuteOrUnMute();
	 }
#endif

	return TRUE;
}


void SpdifPlayRun(uint16_t msgId)
{
#ifdef CFG_FUNC_AUDIO_EFFECT_EN
	if(IsEffectChange == 1)
	{
		IsEffectChange = 0;
		AudioAPPDigitalGianProcess(mainAppCt.SysCurrentMode);
		AudioEffectsLoadInit(0, mainAppCt.EffectMode);
	}
	if(IsEffectChangeReload == 1)
	{
		IsEffectChangeReload = 0;
		AudioEffectsLoadInit(1, mainAppCt.EffectMode);
		AudioAPPDigitalGianProcess(mainAppCt.SysCurrentMode);
		AudioEffectsLoadInit(0, mainAppCt.EffectMode);
	}
#ifdef CFG_EFFECT_PARAM_UPDATA_BY_ACPWORKBENCH
	if(EffectParamFlashUpdataFlag == 1)
	{
		EffectParamFlashUpdataFlag = 0;
		EffectParamFlashUpdata();
	}
#endif
#endif

	switch(msgId)
	{
		default:
			{
				SpdifPlayRunning(msgId);
#ifdef CFG_APP_COAXIAL_MODE_EN
				if(GetSystemMode() == ModeCoaxialAudioPlay)
				{
					if(SPDIF_FlagStatusGet(SYNC_FLAG_STATUS) || (!SPDIF_FlagStatusGet(LOCK_FLAG_STATUS)))
					{
						SPDIF_RXInit(1, 0, 0);
						SPDIF_ModuleEnable();
					}
				}
#endif

				if(SpdifLockFlag && !SPDIF_FlagStatusGet(LOCK_FLAG_STATUS))
				{
					APP_DBG("SPDIF RX UNLOCK!\n");
					SpdifLockFlag = FALSE;
					AudioCoreSourceDisable(SPDIF_SOURCE_NUM);
				}
				if(!SpdifLockFlag && SPDIF_FlagStatusGet(LOCK_FLAG_STATUS))
				{
					APP_DBG("SPDIF RX LOCK!\n");
					SpdifLockFlag = TRUE;
					if(IsAudioPlayerMute() == FALSE)
					{
						HardWareMuteOrUnMute();
					}
					vTaskDelay(20);
					AudioCoreSourceEnable(SPDIF_SOURCE_NUM);
					///AudioCoreSourceUnmute(SPDIF_SOURCE_NUM, TRUE, TRUE);
					if(IsAudioPlayerMute() == TRUE)
					{
						HardWareMuteOrUnMute();
					}
				}

				//���SPDIF RX�������Ƿ�ı�
				if(SpdifLockFlag == TRUE)
				{
					if(samplerate != SPDIF_SampleRateGet())
					{
						samplerate = SPDIF_SampleRateGet();

						SpdifPlayCt->SpdifSampleRate = samplerate;
						APP_DBG("Get SampleRate: %d\n", (int)SpdifPlayCt->SpdifSampleRate);
						SpdifSampleRateChange();
					}

					SpdifDataCarry();
				}
				//�������ȼ�����Ϊ4,ͨ�����͸�����������AudioCore service��Ч������
				{
					MessageContext		msgSend;
					msgSend.msgId		= MSG_NONE;
					MessageSend(GetAudioCoreServiceMsgHandle(), &msgSend);
				}
			}
			break;
	}
}

bool SpdifPlayDeinit(void)
{
	APP_DBG("Spdif Play Deinit\n");
	if(SpdifPlayCt == NULL)
	{
		return FALSE;
	}

	if(IsAudioPlayerMute() == FALSE)
	{
		HardWareMuteOrUnMute();
	}
	
	PauseAuidoCore();
	AudioCoreProcessConfig((void*)AudioNoAppProcess);
	AudioCoreSourceDisable(SPDIF_SOURCE_NUM);//SPDIF_SOURCE_NUM
	AudioCoreSourceDeinit(SPDIF_SOURCE_NUM);

#ifdef CFG_FUNC_AUDIO_EFFECT_EN
	AudioEffectsDeInit();
#endif
	
	SPDIF_ModuleDisable();
	DMA_InterruptFlagClear(PERIPHERAL_ID_SPDIF_RX, DMA_DONE_INT);
	DMA_InterruptFlagClear(PERIPHERAL_ID_SPDIF_RX, DMA_THRESHOLD_INT);
	DMA_InterruptFlagClear(PERIPHERAL_ID_SPDIF_RX, DMA_ERROR_INT);
	DMA_ChannelDisable(PERIPHERAL_ID_SPDIF_RX);

	SPDIF_AnalogModuleDisable();

#ifdef CFG_APP_COAXIAL_MODE_EN
	GPIO_PortAModeSet(SPDIF_COAXIAL_INDEX, 0);
#endif
#ifdef CFG_APP_OPTICAL_MODE_EN
	GPIO_PortAModeSet(SPDIF_OPTICAL_INDEX, 0);
#endif

	if(SpdifPlayCt->SpdifCarry != NULL)
	{
		osPortFree(SpdifPlayCt->SpdifCarry);
		SpdifPlayCt->SpdifCarry = NULL;
	}
#if (defined(CFG_AUDIO_WIDTH_24BIT) && !defined(BT_TWS_SUPPORT) )
	if(SpdifPlayCt->SpdifCarry24 != NULL)
	{
		osPortFree(SpdifPlayCt->SpdifCarry24);
		SpdifPlayCt->SpdifCarry24 = NULL;
	}
#endif

	if(SpdifPlayCt->SpdifPcmFifo != NULL)
	{
		osPortFree(SpdifPlayCt->SpdifPcmFifo);
		SpdifPlayCt->SpdifPcmFifo = NULL;
	}

	if(SpdifPlayCt->SpdifPwcFIFO != NULL)
	{
		osPortFree(SpdifPlayCt->SpdifPwcFIFO);
		SpdifPlayCt->SpdifPwcFIFO = NULL;
	}
	SpdifPlayCt->AudioCoreSpdif = NULL;

#if 1//def CFG_FUNC_MIXER_SRC_EN
	if(SpdifPlayCt->ResamplerCt != NULL)
	{
		osPortFree(SpdifPlayCt->ResamplerCt);
		SpdifPlayCt->ResamplerCt = NULL;
	}
	if(SpdifPlayCt->resampleOutBuf != NULL)
	{
		osPortFree(SpdifPlayCt->resampleOutBuf);
		SpdifPlayCt->resampleOutBuf = NULL;
	}
#endif

	ModeCommonDeinit();//ͨ·ȫ���ͷ�
	
	osPortFree(SpdifPlayCt);
	SpdifPlayCt = NULL;
	
	return TRUE;
}
#endif 

