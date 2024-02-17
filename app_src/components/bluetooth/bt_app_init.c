/**
 **************************************************************************************
 * @file    bluetooth_init.c
 * @brief   
 *
 * @author  KK
 * @version V1.0.0
 *
 * $Created: 2021-4-18 18:00:00$
 *
 * @Copyright (C) Shanghai Mountain View Silicon Co.,Ltd. All rights reserved.
 **************************************************************************************
 */
#include "delay.h"
#include "debug.h"
#include "chip_info.h"
#include "app_config.h"
#include "bt_manager.h"
#include "bt_app_init.h"
#include "bt_app_common.h"
#include "bt_app_ddb_info.h"
#include "bb_api.h"
#include "bt_spp_api.h"
#include "bt_em_config.h"
#include "bt_common_api.h"

extern BT_CONFIGURATION_PARAMS		*btStackConfigParams;
extern uint8_t Efuse_ReadData(uint8_t Addr);

//EM BT HOST CONFIG DATA
const BT_HOST_PARAM App_Bt_Host_config = 
{
	BT_DEVICE_NUMBER,
	BT_SCO_NUMBER
};

/***********************************************************************************
 * @brief	Get Bt Address  �������������ַ
 * @param	device Address
 *			mode: 1=from efuse	0=default
 * @return	
 **********************************************************************************/
bool GetBtDefaultAddr(uint8_t *devAddr)
{
	uint8_t i;
	uint32_t sum=0;
	uint32_t random_mac = 0;

	if(devAddr == NULL)
		return FALSE;

	//1.frome efuse 2-6 and sum
	{
		for(i=0;i<5;i++)
		{
			*(devAddr+i) = Efuse_ReadData(2+i);
			sum += *(devAddr+i);
		}
		*(devAddr+5) = (uint8_t)(sum&0x000000ff);

		if((*devAddr == 0)&&(*(devAddr+1) == 0)&&(*(devAddr+2) == 0)&&(*(devAddr+3) == 0)&&(*(devAddr+4) == 0))
		{
			DBG("efuse is null\n");
			//2.generate a random address
			{
				//uint8_t addr[6] = BT_ADDRESS;
				random_mac = Chip_RandomSeedGet();
				*(devAddr+2) = (uint8_t)(random_mac&0xff);
				*(devAddr+3) = (uint8_t)((random_mac>>8)&0xff);
				*(devAddr+4) = (uint8_t)((random_mac>>16)&0xff);
				*(devAddr+5) = (uint8_t)((random_mac>>24)&0xff);
			}
		}
	}

	extern uint8_t CheckMatch(uint8_t *bdAddr);
	CheckMatch(devAddr);

	return TRUE;
}

/***********************************************************************************
 * 
 **********************************************************************************/
static void prinfBtConfigParams(void)
{
	APP_DBG("**********\nLocal Device Infor:\n");

	//bluetooth name and address
	APP_DBG("Bt Name:%s\n", btStackConfigParams->bt_LocalDeviceName);
	APP_DBG("FlashBtAddre(NAP-UAP-LAP):");
	APP_DBG("%02x:%02x:%02x:%02x:%02x:%02x\n", 
			btStackConfigParams->bt_LocalDeviceAddr[0],
			btStackConfigParams->bt_LocalDeviceAddr[1],
			btStackConfigParams->bt_LocalDeviceAddr[2],
			btStackConfigParams->bt_LocalDeviceAddr[3],
			btStackConfigParams->bt_LocalDeviceAddr[4],
			btStackConfigParams->bt_LocalDeviceAddr[5]
			);
	
	APP_DBG("BtAddr(LAP-UAP-NAP):");
	APP_DBG("%02x:%02x:%02x:%02x:%02x:%02x\n", 
			btManager.btDevAddr[0],
			btManager.btDevAddr[1],
			btManager.btDevAddr[2],
			btManager.btDevAddr[3],
			btManager.btDevAddr[4],
			btManager.btDevAddr[5]
			);
	
	//ble address
	APP_DBG("BleAddr:");
	APP_DBG("%02x:%02x:%02x:%02x:%02x:%02x\n", 
			btStackConfigParams->ble_LocalDeviceAddr[0],
			btStackConfigParams->ble_LocalDeviceAddr[1],
			btStackConfigParams->ble_LocalDeviceAddr[2],
			btStackConfigParams->ble_LocalDeviceAddr[3],
			btStackConfigParams->ble_LocalDeviceAddr[4],
			btStackConfigParams->ble_LocalDeviceAddr[5]
			);
	
	APP_DBG("Freq trim:0x%x\n", btStackConfigParams->bt_trimValue);
	APP_DBG("**********\n");
}

/***********************************************************************************
 * 
 **********************************************************************************/
static int8_t CheckBtAddr(uint8_t* addr)
{
	if(((addr[0]==0x00)&&(addr[1]==0x00)&&(addr[2]==0x00)&&(addr[3]==0x00)&&(addr[4]==0x00)&&(addr[5]==0x00))
			|| ((addr[0]==0xff)&&(addr[1]==0xff)&&(addr[2]==0xff)&&(addr[3]==0xff)&&(addr[4]==0xff)&&(addr[5]==0xff)))
	{
		//������ַ������Ԥ��
		APP_DBG("bt addr error\n");
		return -1;
	}
	else
	{
		return 0;
	}
}

static int8_t CheckBtName(uint8_t* nameString)
{
	if(((nameString[0]==0x00)&&(nameString[1]==0x00)&&(nameString[2]==0x00))
			|| ((nameString[0]==0xff)&&(nameString[1]==0xff)&&(nameString[2]==0xff)))
	{
		//�������Ʋ�����Ԥ��
		APP_DBG("bt name error\n");
		return -1;
	}
	else
	{
		return 0;
	}
}

static int8_t CheckBtParamHeader(uint8_t* header)
{
	if((header[0]=='M')&&(header[1]=='V')&&(header[2]=='B')&&(header[3]=='T'))
	{
		//��������header����Ԥ��
		return 0;
	}
	else
	{
		APP_DBG("header error\n");
		return -1;
	}
}

/***********************************************************************************
 * ��flash��Load�������ò���
 **********************************************************************************/
void LoadBtConfigurationParams(void)
{
	int8_t ret = 0;
	uint8_t paramsUpdate = 0;
	if(btStackConfigParams == NULL)
		return;

	ret = BtDdb_LoadBtConfigurationParams(btStackConfigParams);
	if(ret == -3)
	{
		//��ȡ�쳣��read again
		ret = BtDdb_LoadBtConfigurationParams(btStackConfigParams);
		if(ret == -3)
		{
			APP_DBG("bt database read error!\n");
			return;
		}
	}
	
	//BT ADDR
	ret = CheckBtAddr(btStackConfigParams->bt_LocalDeviceAddr);
	if(ret != 0)
	{
		paramsUpdate = 1;
		GetBtDefaultAddr(btStackConfigParams->bt_LocalDeviceAddr);
	}
	
	//��ADDR���з������:btStackConfigParams->bt_LocalDeviceAddr(NAP-UAP-LAP)->btManager.btDevAddr(LAP-UAP-NAP)
	btManager.btDevAddr[5]=btStackConfigParams->bt_LocalDeviceAddr[0];
	btManager.btDevAddr[4]=btStackConfigParams->bt_LocalDeviceAddr[1];
	btManager.btDevAddr[3]=btStackConfigParams->bt_LocalDeviceAddr[2];
	btManager.btDevAddr[2]=btStackConfigParams->bt_LocalDeviceAddr[3];
	btManager.btDevAddr[1]=btStackConfigParams->bt_LocalDeviceAddr[4];
	btManager.btDevAddr[0]=btStackConfigParams->bt_LocalDeviceAddr[5];
	
	//BLE ADDR
	ret = CheckBtAddr(btStackConfigParams->ble_LocalDeviceAddr);
	if(ret != 0)
	{
		paramsUpdate = 1;
		
		//ble address
		//ble name:ͨ������BLE�㲥��Ϣ������(ble_app_func.c)
		memcpy(btStackConfigParams->ble_LocalDeviceAddr, btStackConfigParams->bt_LocalDeviceAddr,6);
		btStackConfigParams->ble_LocalDeviceAddr[0] |= 0xc0;
		btStackConfigParams->ble_LocalDeviceAddr[5] |= 0xc0;
	}

#if 0
	//BT name �������ư���BT_NAME
	strcpy((void *)btStackConfigParams->bt_LocalDeviceName, BT_NAME);
#else
	//BT name �������ƴ�flash�л�ȡ
	ret = CheckBtName(btStackConfigParams->bt_LocalDeviceName);
	if(ret != 0)
	{
		strcpy((void *)btStackConfigParams->bt_LocalDeviceName, sys_parameter.bt_LocalDeviceName);
	}
#endif //#if defined(BT_TWS_SUPPORT)
    
	//BLE name �������ư���BT_NAME
	//strcpy((void *)btStackConfigParams->ble_LocalDeviceName, BLE_NAME);
	//BLE name �������ƴ�flash�л�ȡ
	ret = CheckBtName(btStackConfigParams->ble_LocalDeviceName);
	if(ret != 0)
	{
		strcpy((void *)btStackConfigParams->ble_LocalDeviceName, sys_parameter.ble_LocalDeviceName);
	}
    
	//BT PARAMS
	ret = CheckBtParamHeader(btStackConfigParams->bt_ConfigHeader);
	if(ret != 0)
	{
		APP_DBG("used default bt trim value\n");
		//��ƥ��header�쳣ʱ,������Ƶƫֵ�޸�ΪĬ�ϲ���
		//note:��ʹ��Ĭ�ϲ���ʱ��trimValueһ������Ϊ0xff������ᵼ��bb������������
		if(CHIP_VERSION_ECO0 == Chip_Version())
			btStackConfigParams->bt_trimValue = sys_parameter.BtTrimECO0;
		else
			btStackConfigParams->bt_trimValue = sys_parameter.BtTrim;
	}
	
	//�����������ò���,��ʱ���պ궨��Ĭ�ϵĲ����������� bt_config.h,Ƶƫֵ����flash������
	//���������Ҫ�ܶ�̬����,������λ�������޸ĵ�,����Ҫ���浽flash�н��й���
	btStackConfigParams->bt_TxPowerValue = sys_parameter.bt_TxPowerLevel;
	
	btStackConfigParams->bt_SupportProfile = GetSupportProfiles();
	
	//simple Pairing enable
	//��simplePairing=1ʱ,pinCode��Ч;��֮��Ȼ
	btStackConfigParams->bt_simplePairingFunc = sys_parameter.bt_SimplePairingEnable;
	btStackConfigParams->bt_pinCodeLen = strlen(sys_parameter.bt_PinCode);
	strcpy((char *)btStackConfigParams->bt_pinCode,sys_parameter.bt_PinCode);

	if(paramsUpdate)
	{
		APP_DBG("save bt params to flash\n");
		//����Ĭ�ϵ����ò�����flash����������bt_ConfigHeader��bt_trimValue
		BtDdb_InitBtConfigurationParams(btStackConfigParams);
	}
	
	prinfBtConfigParams();
}

/***********************************************************************************
 * ����BB�Ĳ���
 **********************************************************************************/
void ConfigBtBbParams(BtBbParams *params)
{
	if(params == NULL)
		return;

	memset(params, 0 ,sizeof(BtBbParams));

	params->localDevName = (uint8_t *)btStackConfigParams->bt_LocalDeviceName;
	memcpy(params->localDevAddr, btManager.btDevAddr, BT_ADDR_SIZE);
	params->freqTrim = btStackConfigParams->bt_trimValue;

	//em config
	params->em_start_addr = BB_EM_START_PARAMS;

	//agc config
	params->pAgcDisable = 0; //0=auto agc;	1=close agc
	params->pAgcLevel = 1;

	//sniff config
	params->pSniffNego = 0;//1=open;  0=close
	params->pSniffDelay = 0;
	params->pSniffInterval = 0x320;//500ms
	params->pSniffAttempt = 0x01;
	params->pSniffTimeout = 0x01;
	
	//params->bbSniffNotify = NULL;

	SetRfTxPwrMaxLevel(sys_parameter.bt_TxPowerLevel, sys_parameter.bt_PagePowerLevel);

	BtSetLinkSupervisionTimeout(BT_LSTO_DFT);
}


/***********************************************************************************
 * ����HOST�Ĳ���
 **********************************************************************************/
static void ConfigBtStackParams(BtStackParams *stackParams)
{
	uint32_t pCod = 0;
	if(stackParams == NULL)
		return ;

	memset(stackParams, 0 ,sizeof(BtStackParams));

	/* Set support profiles */
	stackParams->supportProfiles = GetSupportProfiles();

	/* Set local device name */
	stackParams->localDevName = (uint8_t *)btStackConfigParams->bt_LocalDeviceName;

	/* Set callback function */
	stackParams->callback = BtStackCallback;
	stackParams->scoCallback = NULL;//GetScoDataFromApp;

	//simple pairing
	stackParams->btSimplePairing = btStackConfigParams->bt_simplePairingFunc;
	if((btStackConfigParams->bt_pinCodeLen)&&(btStackConfigParams->bt_pinCodeLen<17))
	{
		stackParams->btPinCodeLen = btStackConfigParams->bt_pinCodeLen;
		memcpy(stackParams->btPinCode, btStackConfigParams->bt_pinCode, stackParams->btPinCodeLen);
	}
	else
	{
		APP_DBG("ERROR:pin code len %d\n", btStackConfigParams->bt_pinCodeLen);
		stackParams->btSimplePairing = 1;//�����������
	}

	//class of device
	//headset:ƻ���ֻ�����ʾ�豸�ĵ�ص���
	if(sys_parameter.bt_SimplePairingEnable)
		pCod = COD_AUDIO | COD_MAJOR_AUDIO | COD_MINOR_AUDIO_HEADSET | COD_RENDERING;
	else
	//Note:1.ƻ���ֻ���BP10��HFPЭ��,����Ҫ��ͬ��ͨѶ¼�ĵ���,����Ҫ���豸��������Ϊhandsfree,��������ʾ��ص���
	//2.ʹ��pin code�Ļ�,����Ҫ��class of device����Ϊhandsfree,����ĳЩ�ֻ���֧��
	pCod = COD_AUDIO | COD_MAJOR_AUDIO | COD_MINOR_AUDIO_HANDSFREE | COD_RENDERING;

	//���粻��ҪHFP,�ͻ������ֻ�����ʱ�����绰����Ȩ�޻�ȡ,����Ҫ����Ϊ HIFI audio
	//pCod = COD_AUDIO | COD_MAJOR_AUDIO | COD_MINOR_AUDIO_HIFIAUDIO | COD_RENDERING;
	
#if (BT_HID_SUPPORT == ENABLE)
	pCod |= (COD_MAJOR_PERIPHERAL | COD_MINOR_PERIPH_KEYBOARD);
#endif
	SetBtClassOfDevice(pCod);

#ifdef BT_TWS_SUPPORT
	stackParams->twsFeatures.twsAppCallback = BtTwsCallback;
	#if (TWS_PAIRING_MODE == CFG_TWS_ROLE_RANDOM)
	stackParams->twsFeatures.twsSimplePairingCfg = DISABLE;
	btManager.twsSimplePairingCfg = DISABLE;
	#else
	stackParams->twsFeatures.twsSimplePairingCfg = ENABLE;
	btManager.twsSimplePairingCfg = ENABLE;
	#endif
	stackParams->twsFeatures.twsRoleCfg = TWS_PAIRING_MODE;

#ifdef TWS_FILTER_USER_DEFINED
	strcpy((void *)btManager.TwsFilterInfor, TWS_FILTER_INFOR);
#endif
#endif//defined(BT_TWS_SUPPORT)

#if BT_HFP_SUPPORT == ENABLE
	/* HFP features */
	stackParams->hfpFeatures.wbsSupport = BT_HFP_SUPPORT_WBS;
	stackParams->hfpFeatures.hfpAudioDataFormat = BT_HFP_AUDIO_DATA;
	stackParams->hfpFeatures.hfpAppCallback = BtHfpCallback;
#else
	stackParams->hfpFeatures.hfpAppCallback = NULL;
#endif
	
#if BT_A2DP_SUPPORT == ENABLE
	/* A2DP features */
	//stackParams->a2dpFeatures.a2dpAudioDataFormat = BT_A2DP_AUDIO_DATA;
#if (defined(BT_AUDIO_AAC_ENABLE) && defined(USE_AAC_DECODER))
	stackParams->a2dpFeatures.a2dpAudioDataFormat = (A2DP_AUDIO_DATA_SBC | A2DP_AUDIO_DATA_AAC);
#else
	stackParams->a2dpFeatures.a2dpAudioDataFormat = (A2DP_AUDIO_DATA_SBC);
#endif
	stackParams->a2dpFeatures.a2dpAppCallback = BtA2dpCallback;

	/* AVRCP features */
	stackParams->avrcpFeatures.supportAdvanced = 1;
#if ((BT_AVRCP_PLAYER_SETTING == ENABLE)||(BT_AVRCP_VOLUME_SYNC == ENABLE))
	stackParams->avrcpFeatures.supportTgSide = 1;
#else
	stackParams->avrcpFeatures.supportTgSide = 0;
#endif
	stackParams->avrcpFeatures.supportSongTrackInfo = BT_AVRCP_SONG_TRACK_INFOR;
	stackParams->avrcpFeatures.supportPlayStatusInfo = BT_AVRCP_SONG_PLAY_STATE;
	stackParams->avrcpFeatures.avrcpAppCallback = BtAvrcpCallback;

#else
	stackParams->a2dpFeatures.a2dpAppCallback = NULL;
	stackParams->avrcpFeatures.avrcpAppCallback = NULL;
#endif
	
}

/***********************************************************************************
 * ��ʼ������HOST
 **********************************************************************************/
bool BtStackInit(void)
{
//	bool ret;
	int32_t retInit=0;

	BtStackParams	stackParams;

	BtManageInit();

	//BTStatckSetPageTimeOutValue(BT_PAGE_TIMEOUT); 

	//BTHostLinkNumConfig(BT_DEVICE_NUMBER, BT_SCO_NUMBER);
//	BTHostParamsConfig(&App_Bt_Host_config);

	ConfigBtStackParams(&stackParams);

	retInit = BTStackRunInit(&stackParams);
	if(retInit != 0)
	{
		APP_DBG("Bt Stack Init ErrCode [%x]\n", (int)retInit);
		return FALSE;
	}

#if ( BT_A2DP_SUPPORT == ENABLE )
	retInit = A2dpAppInit(&stackParams.a2dpFeatures);
    if(retInit == 0)
	{
		APP_DBG("A2dp Init ErrCode [%x]\n", (int)retInit);
		return FALSE;
	}

	retInit = AvrcpAppInit(&stackParams.avrcpFeatures);
	if(retInit == 0)
	{
		APP_DBG("Avrcp Init ErrCode [%x]\n", (int)retInit);
		return FALSE;
	}
	
#endif

#if (BT_SPP_SUPPORT == ENABLE ||(defined(CFG_FUNC_BT_OTA_EN)))
	retInit = SppAppInit(BtSppCallback);
	if(retInit == 0)
	{
		APP_DBG("Spp Init Error!\n");
		return FALSE;
	}
#endif

#if (BT_HFP_SUPPORT == ENABLE)
	retInit = HfpAppInit(&stackParams.hfpFeatures);
	if(retInit == 0)
	{
		APP_DBG("Hfp Init ErrCode [%x]\n", (int)retInit);
		return FALSE;
	}
#endif

	return TRUE;
}

/***********************************************************************************
 * ����ʼ������HOST
 **********************************************************************************/
bool BtStackUninit(void)
{
	int32_t ret=0;
	BTStackRunUninit();
	
	ret = BTStackMemFree();
	if(ret == -1)
	{
		return FALSE;
	}
	return TRUE;
}

/***********************************************************************************
 * 
 **********************************************************************************/
void BtStackRunNotify(void)
{
//	OSQueueMsgSend(MSG_NEED_BT_STACK_RUN, NULL, 0, MSGPRIO_LEVEL_HI, 0);
}



