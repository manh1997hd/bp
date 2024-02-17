/**
 **************************************************************************************
 * @file    tws_slave_mode.c
 * @brief   
 *
 * @author  Owen
 * @version V1.0.0
 *
 * $Created: 2021-7-15 13:06:47$
 *
 * @Copyright (C) 2016, Shanghai Mountain View Silicon Co.,Ltd. All rights reserved.
 **************************************************************************************
 */
#include <string.h>
#include <nds32_intrinsic.h>
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
#include "dac_interface.h"
#include "i2s_interface.h"
#include "audio_core_api.h"
#include "audio_core_service.h"
#include "decoder.h"
#include "remind_sound.h"
#include "main_task.h"
#include "audio_effect.h"
#include "powercontroller.h"
#include "deepsleep.h"
#include "backup_interface.h"
#include "breakpoint.h"
#include "otg_device_hcd.h"
#include "otg_device_standard_request.h"
#include "audio_vol.h"
#include "ctrlvars.h"
#include "reset.h"
#include "audio_effect_flash_param.h"
#include <tws_mode.h>
#include "bt_manager.h"
#include "bt_tws_api.h"
#include "bt_app_tws.h"
#include "bb_api.h"
#include "bt_app_connect.h"
#include "mode_task_api.h"
#include "bt_app_tws_connect.h"

#ifdef BT_TWS_SUPPORT

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

static const uint8_t DmaChannelMap[29] = {
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
	255,//PERIPHERAL_ID_SDPIF_RX,		//8 SPDIF_RX /TX same chanell
	255,//PERIPHERAL_ID_SDPIF_TX,		//8 SPDIF_RX /TX same chanell
	255,//PERIPHERAL_ID_SPIM_RX,		//9
	255,//PERIPHERAL_ID_SPIM_TX,		//10
	
#if (defined(CFG_DUMP_DEBUG_EN)&&(CFG_DUMP_UART_TX_PORT_GROUP == 0))
	7,//PERIPHERAL_ID_UART0_TX,			//11
#else
	255,//PERIPHERAL_ID_UART0_TX,		//11
#endif
	255,//PERIPHERAL_ID_UART1_RX,		//12
	
#if (defined(CFG_DUMP_DEBUG_EN)&&(CFG_DUMP_UART_TX_PORT_GROUP == 1))
	7,//PERIPHERAL_ID_UART1_TX,			//13
#else
	255,//PERIPHERAL_ID_UART1_TX,		//13
#endif

	255,//PERIPHERAL_ID_TIMER4,			//14
	255,//PERIPHERAL_ID_TIMER5,			//15
	255,//PERIPHERAL_ID_TIMER6,			//16
	0,//PERIPHERAL_ID_AUDIO_ADC0_RX,	//17
	1,//PERIPHERAL_ID_AUDIO_ADC1_RX,	//18
	2,//PERIPHERAL_ID_AUDIO_DAC0_TX,	//19
	3,//PERIPHERAL_ID_AUDIO_DAC1_TX,	//20

#if defined (CFG_FUNC_I2S_MIX_MODE) && defined (CFG_RES_AUDIO_I2S0IN_EN)
	6,//PERIPHERAL_ID_I2S0_RX,			//21
#else
	255,//PERIPHERAL_ID_I2S0_RX,		//21
#endif

#if	((defined(CFG_RES_AUDIO_I2SOUT_EN )&&(CFG_RES_I2S == 0)) || defined(CFG_RES_AUDIO_I2S0OUT_EN))
	7,//PERIPHERAL_ID_I2S0_TX,			//22
#else	
	255,//PERIPHERAL_ID_I2S0_TX,		//22
#endif	

#if defined (CFG_FUNC_I2S_MIX_MODE) && defined (CFG_RES_AUDIO_I2S1IN_EN)
	4,//PERIPHERAL_ID_I2S1_RX,			//23
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

#define TWS_SLAVE_SOURCE_NUM				APP_SOURCE_NUM
#define TWS_SLAVE_LINK_TIMEOUT				(100*60*3)

typedef struct _TwsSlavePlayContext
{
	uint8_t				runflag;
	uint8_t				umuteflag;
	TIMER				umuteTime;
	
	uint32_t			TwsBufMuteTimeout;//��������dac muteʱ���ӻ���Ҫmute ֱ��fifo������ϣ� ==0 ʱδ����

	uint32_t			twsSlaveLinkTimeout;
	uint32_t			twsSlaveDiscTimeout;//�Ͽ���ȴ�ʱ��ȷ���������Ͽ�����linkloss
	uint32_t 			mode_start_tick;
	uint32_t 			mode_start_busy;
}TwsSlavePlayContext;

static  TwsSlavePlayContext*		gTwsSlavePlayCt;
const int16_t jingyin[512]={0,0,0};
TWS_CONFIG	gTwsCfg;
extern uint32_t gSysTick;

/**********************************************************************************************
 *
 * 
 *
 *********************************************************************************************/
static void TwsSlavePlayTimeCheck(void)
{
	if(btManager.twsState == BT_TWS_STATE_NONE)
	{
		gTwsSlavePlayCt->twsSlaveLinkTimeout++;
		if(gTwsSlavePlayCt->twsSlaveLinkTimeout >= TWS_SLAVE_LINK_TIMEOUT)
		{
			//3���������ӳ�ʱ�˳�
			gTwsSlavePlayCt->twsSlaveLinkTimeout=0;
			TwsSlaveModeExit();
		}

		if(gTwsSlavePlayCt->twsSlaveDiscTimeout)
		{
			gTwsSlavePlayCt->twsSlaveDiscTimeout--;
			if(gTwsSlavePlayCt->twsSlaveDiscTimeout == 0)
			{
				TwsSlaveModeExit();
			}
		}
	}
	else
	{
		gTwsSlavePlayCt->twsSlaveLinkTimeout = 0;
	}
}

/**********************************************************************************************
 *
 * 
 *
 *********************************************************************************************/
#if (LINEIN_INPUT_CHANNEL != ANA_INPUT_CH_LINEIN3)
static uint16_t TwsSlavePlayDataGet(void* Buf, uint16_t Len)
{
	memset(Buf, 0, Len);
	return Len;
}

static uint16_t TwsSlavePlayDataLenGet(void)
{
	//������
	return AudioCoreFrameSizeGet(DefaultNet);//��������
}
#endif
/**********************************************************************************************
 *
 * 
 *
 *********************************************************************************************/
void TwsSlaveFifoMuteTimeSet(void)
{
	if(gTwsSlavePlayCt == NULL)
	{
		APP_DBG("gTwsSlavePlayCt is NULL! %d\n",__LINE__);
		return;
	}
	gTwsSlavePlayCt->TwsBufMuteTimeout = GetSysTick1MsCnt() + (TWS_STATRT_PLAY_FRAM * 128 * 1000 ) / CFG_PARA_SAMPLE_RATE;
	gTwsSlavePlayCt->runflag = 1;
	gTwsSlavePlayCt->umuteflag = 0;
	TimeOutSet(&gTwsSlavePlayCt->umuteTime, 0xfffffff);//��һ����������ֵ��
}

/**********************************************************************************************
 *
 * 
 *
 *********************************************************************************************/
//�յ�unmute ����� ����ѳ���fifo��ȾͿ��ٽ�mute����֮���ݲ�����ʱ��mute��
void TwsSlaveFifoUnmuteSet(void)
{
	if(gTwsSlavePlayCt == NULL)
	{
		APP_DBG("gTwsSlavePlayCt is NULL! %d\n",__LINE__);
		return;
	}

	gTwsSlavePlayCt->umuteflag = 0;
	if(gTwsSlavePlayCt->runflag == 1 && gTwsSlavePlayCt->umuteflag == 0)
	{
		if(gTwsSlavePlayCt->TwsBufMuteTimeout)
		{
			if(GetSysTick1MsCnt() >= gTwsSlavePlayCt->TwsBufMuteTimeout + 1)
			{
				TimeOutSet(&gTwsSlavePlayCt->umuteTime, 1);//ּ�ڴ�����mute.
			}
			else
			{
				TimeOutSet(&gTwsSlavePlayCt->umuteTime, gTwsSlavePlayCt->TwsBufMuteTimeout - GetSysTick1MsCnt());
			}
			gTwsSlavePlayCt->TwsBufMuteTimeout = 0;//���� fifo mute��ʱ���ơ�
		}
	}
}

/**********************************************************************************************
 * @brief       
 * @param       
 * @return      
 *********************************************************************************************/
bool TwsSlavePlayResMalloc(uint16_t SampleLen)
{
	//InCore1 buf
	return TRUE;
}

/**********************************************************************************************
 * @func        TwsSlavePlay_Init
 * @brief       TwsSlaveģʽ�������ã���Դ��ʼ��
 * @param       MessageHandle 
 * @Output      None
 * @return      bool
 * @Others      ����顢Adc��Dac��AudioCore����
 * @Others      ��������Adc��audiocore���к���ָ�룬audioCore��Dacͬ����audiocoreService����������
 *********************************************************************************************/
bool TwsSlavePlayInit(void)
{
	APP_DBG("TwsSlave:init\n");

	//dma channel map
	DMA_ChannelAllocTableSet((uint8_t *)DmaChannelMap);//TwsSlave
#ifdef CFG_FUNC_AUDIO_EFFECT_EN
	//��Ч����������ȷ��ϵͳ֡�������޸ģ�sam, mark
#endif
	if(!ModeCommonInit())
	{
		return FALSE;
	}
	//Task & App Config
	gTwsSlavePlayCt = (TwsSlavePlayContext*)osPortMalloc(sizeof(TwsSlavePlayContext));
	if(gTwsSlavePlayCt == NULL)
	{
		return FALSE;
	}
	memset(gTwsSlavePlayCt, 0, sizeof(TwsSlavePlayContext));
	
	if(!TwsSlavePlayResMalloc(AudioCoreFrameSizeGet(DefaultNet)))
	{
		APP_DBG("TwsSlavePlay Res Error!\n");
		return FALSE;
	}
	
	//Audio init
	//note Soure0.��sink0�Ѿ���main app�����ã���Ҫ��������
	//Core Soure1.Para
	AudioCoreIO	AudioIOSet;
	memset(&AudioIOSet, 0, sizeof(AudioCoreIO));
	AudioIOSet.Adapt = STD;
	AudioIOSet.Sync = FALSE;
	AudioIOSet.Channels = 2;
	AudioIOSet.Net = DefaultNet;
#if (LINEIN_INPUT_CHANNEL == ANA_INPUT_CH_LINEIN3)
	AudioIOSet.DataIOFunc = AudioADC1DataGet;
	AudioIOSet.LenGetFunc = AudioADC1DataLenGet;
#else
	AudioIOSet.DataIOFunc = TwsSlavePlayDataGet;
	AudioIOSet.LenGetFunc = TwsSlavePlayDataLenGet;//������;
#endif

	if(!AudioCoreSourceInit(&AudioIOSet, TWS_SLAVE_SOURCE_NUM))
	{
		DBG("twsplay source error!\n");
		return FALSE;
	}

#ifdef CFG_FUNC_AUDIO_EFFECT_EN
	AudioCoreProcessConfig((void*)AudioMusicProcess);
#else
	AudioCoreProcessConfig((void*)AudioBypassProcess);
#endif

	gTwsSlavePlayCt->runflag = 0;
	gTwsSlavePlayCt->umuteflag = 0;
	gTwsSlavePlayCt->TwsBufMuteTimeout = 0;

		
#if (TWS_PAIRING_MODE != CFG_TWS_PEER_SLAVE)
	if(btManager.twsState != BT_TWS_STATE_CONNECTED)
	{
		TwsSlaveModeExit();
		APP_DBG("TwsSlavePlayInit: Tws Disconnect!\n");
	}
#endif

	gTwsSlavePlayCt->runflag = 1;
	gTwsSlavePlayCt->umuteflag = 0;
	TimeOutSet(&gTwsSlavePlayCt->umuteTime, 200);
	
	
	AudioCoreSourceEnable(TWS_SLAVE_SOURCE_NUM);

#ifdef CFG_AUTO_ENTER_TWS_SLAVE_MODE
	if(btManager.twsState == BT_TWS_STATE_CONNECTED && (!IsIdleModeReady()))
	{
		tws_audio_connect_cmd();
	}
#endif

#ifdef CFG_FUNC_AUDIO_EFFECT_EN
#ifdef CFG_EFFECT_PARAM_IN_FLASH_EN
	mainAppCt.EffectMode = EFFECT_MODE_FLASH_Music;
#else
	mainAppCt.EffectMode = EFFECT_MODE_NORMAL;
#endif
	AudioEffectsLoadInit(1, mainAppCt.EffectMode);
	AudioAPPDigitalGianProcess(mainAppCt.SysCurrentMode);
	AudioEffectsLoadInit(0, mainAppCt.EffectMode);
#endif

	APP_DBG("TwsSlave:run\n");
	gTwsSlavePlayCt->mode_start_tick = gSysTick;
	gTwsSlavePlayCt->mode_start_busy = 1;
	
	if(IsAudioPlayerMute() == TRUE)
	{
	 	HardWareMuteOrUnMute();
	}
	
	return TRUE;
}

/**********************************************************************************************
 * @func        TwsSlavePlayEntrance
 * @brief       ģʽִ������
 * @param       void * param  
 * @Output      None
 * @return      None
 * @Others      ģʽ�����ͽ�������
 *********************************************************************************************/
void TwsSlavePlayRun(uint16_t msgId)
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
	if(gTwsSlavePlayCt->mode_start_busy == 1)
	{
		if((gSysTick - gTwsSlavePlayCt->mode_start_tick) > 3000)
		{
			if(tws_audio_state_get() == TWS_DISCONNECT)
			{
				tws_sync_reinit();
				gTwsSlavePlayCt->mode_start_tick = gSysTick;
				gTwsSlavePlayCt->mode_start_busy = 2;
			}
			else
			{
				gTwsSlavePlayCt->mode_start_busy = 0;
			}
		}
	}
	if(gTwsSlavePlayCt->mode_start_busy == 2)
	{
		if((gSysTick - gTwsSlavePlayCt->mode_start_tick) > 3000)
		{
			if(tws_audio_state_get() != TWS_DISCONNECT)
			{
				gTwsSlavePlayCt->mode_start_busy = 0;
			}
		}
	}
	switch(msgId)//���棺�ڴ˶δ��룬��ֹ������ʾ���岥λ�á�
	{
		case MSG_BT_TWS_DISCONNECT:
			gTwsSlavePlayCt->twsSlaveDiscTimeout = 5;//50ms�Զ��˳�
			break;

		case MSG_BT_TWS_LINKLOSS:
			gTwsSlavePlayCt->twsSlaveDiscTimeout = 1000;//10s�������ɹ��Զ��˳�
			BtStartReconnectTws(1, 1);
			break;
			
		default:
			CommonMsgProccess(msgId);
			break;
	}

	if(gTwsSlavePlayCt->umuteflag == 0)
	{
		if(gTwsSlavePlayCt->runflag == 1)
		{
			if(IsTimeOut(&gTwsSlavePlayCt->umuteTime))
			{
				gTwsSlavePlayCt->umuteflag = 1;
				if(IsAudioPlayerMute() == TRUE)//��������mute�׶β���mute
				{
					HardWareMuteOrUnMute();
				}
			}
		}
	}

	TwsSlavePlayTimeCheck();
}

/*********************************************************************************************
 * @func        TwsSlavePlayDeinit
 * @brief       
 * @param       void
 * @Output      None
 * @return      None
 * @Others      
 * Record
 *********************************************************************************************/
bool TwsSlavePlayDeinit(void)
{
	#ifdef TWS_SLAVE_MODE_SWITCH_EN
	if(btManager.twsState == BT_TWS_STATE_CONNECTED)
	{
		APP_DBG("tws disconnect\n");
		tws_link_disconnect();
	}
	#endif
	
	if(gTwsSlavePlayCt == NULL)
	{
		return TRUE;
	}	
	
	if(IsAudioPlayerMute() == FALSE)
	{
		HardWareMuteOrUnMute();
	}	
	
	PauseAuidoCore();	

	//ע�⣺AudioCore�����������mainApp�£��˴�ֻ�ر�AudioCoreͨ�������ر�����
	AudioCoreProcessConfig((void*)AudioNoAppProcess);
	AudioCoreSourceDisable(TWS_SLAVE_SOURCE_NUM);
	AudioCoreSourceDeinit(TWS_SLAVE_SOURCE_NUM);

	APP_DBG("Tws Slave:Kill Ct\n");

#ifdef CFG_FUNC_AUDIO_EFFECT_EN
	AudioEffectsDeInit();
#endif
	ModeCommonDeinit();//ͨ·ȫ���ͷ�
	
	osPortFree(gTwsSlavePlayCt);
	gTwsSlavePlayCt = NULL;

	return TRUE;
}


/*****************************************************************************************
 * Enter or Exit Tws Slave Mode
 ****************************************************************************************/
void TwsSlaveModeEnter(void)
{
	MessageContext		msgSend;
	
	APP_DBG("Enter Tws Slave Mode\n");
	
	if(GetSysModeState(ModeTwsSlavePlay) == ModeStateSusend)
	{
		SetSysModeState(ModeTwsSlavePlay,ModeStateReady);
	}
	msgSend.msgId = MSG_DEVICE_SERVICE_TWS_SLAVE_CONNECTED;
	MessageSend(GetMainMessageHandle(), &msgSend);
}
extern uint8_t tws_slave_cap;
/*****************************************************************************************
 * 
 ****************************************************************************************/
void TwsSlaveModeExit(void)
{
	MessageContext		msgSend;
	if(btManager.twsState == BT_TWS_STATE_CONNECTED)
	{
		//tws_link_disconnect();
		BtTwsDisconnectApi();
	}

	APP_DBG("Exit Tws Slave Mode\n");
	msgSend.msgId = MSG_DEVICE_SERVICE_TWS_SLAVE_DISCONNECT;
	MessageSend(GetMainMessageHandle(), &msgSend);
	tws_time_init();
	tws_play = BT_TWS_PLAY_REINIT;
	BtSetFreqTrim(tws_slave_cap);
}

/*****************************************************************************************
 * TWS params init
 ****************************************************************************************/
void TWS_Params_Init(void)
{
	uint32_t tws_mem;
	uint8_t*p;

	gTwsCfg.AudioMode    = TWS_M_L__S_R;
	
	tws_mem = tws_mem_size(TWS_STATRT_PLAY_FRAM,gTwsCfg.AudioMode);
	p = (uint8_t*)osPortMalloc(tws_mem);
	if(p == 0)
	{
		printf("tws mem malloc error\n");
		while(1);
	}
	memset(p, 0, tws_mem);
	printf("tws_mem_size:%lu\n",tws_mem);
	tws_mem_set(p);
	gTwsCfg.PairMode    = 0;	
	gTwsCfg.IsRemindSyncEn = 0;//0:��ʾ�������͸�slave��1:��ʾ�����͸�slaveͬ��������ʾ��
	gTwsCfg.IsEffectEn  = CFG_EFFECT_MUSIC_MASTER;//0=master,slaver �����е�����Ч��1=ֻ��master�е�������Ч���ܣ�slaver����Ϊ���գ�
}


uint16_t TwsSinkDataSet(void* Buf, uint16_t Len)
{
	tws_music_pcm_process(Buf, Len, APP_SOURCE_NUM);
	return Len;
}

uint16_t TwsSinkSpaceLenGet(void)
{
	//������
	return AudioCoreFrameSizeGet(DefaultNet);//��������
}

uint16_t AudioDAC0DataSet_tws(void* Buf, uint16_t Len)
{
	bool gie_ret = GIE_STATE_GET();
	GIE_DISABLE();
	AudioDAC0DataSet(Buf, TwsDeviceSinkSync(Len));
	if(gie_ret)
	{
		GIE_ENABLE();
	}
	return 0;
}

__attribute__((section(".tws_sync_code")))
uint16_t tws_device_space_len(void)
{
#if  (TWS_AUDIO_OUT_PATH	== TWS_IIS0_OUT)
	return AudioI2S0_TX_DataLenGet();
#elif(TWS_AUDIO_OUT_PATH	== TWS_IIS1_OUT)
	return AudioI2S1_TX_DataLenGet();
#elif(TWS_AUDIO_OUT_PATH	== TWS_DAC0_OUT)
	return AudioDAC0DataSpaceLenGet();
#endif
}
#else
uint16_t tws_device_space_len(void)
{
	return 0;
}

void tws_clk_sync(uint32_t new_clk_off,int16_t new_bit_off,uint32_t a,int16_t b)
{
	
}
#endif//#ifdef BT_TWS_SUPPORT

