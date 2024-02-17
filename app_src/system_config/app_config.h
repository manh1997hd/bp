/******************************************************************************
 * @file    app_config.h
 * @author
 * @version V_NEW
 * @date    2021-06-25
 * @maintainer
 * @brief
 ******************************************************************************
 * @attention
 * <h2><center>&copy; COPYRIGHT 2013 MVSilicon </center></h2>
 */
#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include "type.h"
//************************************************************************************************************
//    ��ϵͳĬ�Ͽ���2��ϵͳȫ�ֺ꣬��IDE��������(Build Settings-Compiler-Symbols)���˴���������
//*CFG_APP_CONFIG �� FUNC_OS_EN*/
//************************************************************************************************************

//************************************************************************************************************
//    ���ܿ���˵����
// *CFG_APP_*  : ����ϵͳӦ��ģʽ����������USB U�̲��Ÿ���Ӧ��ģʽ��ѡ��
// *CFG_FUNC_* : �������ܿ���
// *CFG_PARA_* : ϵͳ������������
// *CFG_RES_*  : ϵͳӲ����Դ����
// ************************************************************************************************************


//24bit ��Ч���غ����
//24bit SDK RAM��������������
#define  CFG_AUDIO_WIDTH_24BIT


//****************************************************************************************
//       оƬ�ͺ�ѡ������
// Ӧ�ÿ���ʱ��һ��Ҫѡ���ͺ�,��ֹ��λ����ʾ��mic��linein�����Ӳ���ӿڲ�һ��
//****************************************************************************************
//#define  CFG_CHIP_BP10128
//#define  CFG_CHIP_BP1064A2	// flash���� 2M
//#define  CFG_CHIP_BP1064L2	// flash���� 2M
//#define  CFG_CHIP_BP1048A2	// flash���� 2M
//#define  CFG_CHIP_BP1048B2   	// flash���� 2M
#define  CFG_CHIP_BP1048B2   	// flash���� 1M  �ͻ������аѴ�������� 1M ����
//#define  CFG_CHIP_MT166      	// flash���� 1M  �ͻ������аѴ�������� 1M ����
//#define  CFG_CHIP_BP1032A1   	// flash���� 1M  �ͻ������аѴ�������� 1M ����
//#define  CFG_CHIP_BP1032A2   	// flash���� 2M
//#define  CFG_CHIP_BP1048P2    // flash���� 2M
//#define  CFG_CHIP_BP1048P4    // flash���� 4M

//****************************************************************************************
//       ���1MB FLASHоƬ����
//****************************************************************************************
/**��ƵSDK�汾�ţ�V1.0.0**/
/*0xB1��оƬB1X��01����汾�ţ� 00��С�汾�ţ� 00���û��޶��ţ����û��趨���ɽ�ϲ����ţ���ʵ�ʴ洢�ֽ���1A 01 00 00 ����������sdk�汾*/
/*����flash_bootʱ����flashboot����usercode��boot��������code����(��0xB8��0xB8+0x10000)����ֵ�᲻ͬ��ǰ����burner��¼ʱ�汾��������mva�汾���ע*/
#define	 CFG_SDK_VER_CHIPID			(0xB1)
#define  CFG_SDK_MAJOR_VERSION		(0)
#define  CFG_SDK_MINOR_VERSION		(4)
#define  CFG_SDK_PATCH_VERSION	    (3)

//****************************************************************************************
//       B0,B1����ΪDEBUG���湦��ѡ������
// ˵��:
//    1.���õ�linein4(B0,B1),Ϊ�˷�ֹת��linein4ͨ�����޷����棬��Ҫ�����˺ꣻ
//    2.���õ�linein4(B0,B1),����ʱ��Ҫ�رմ˺꣬����ת����ͨ��ʱ����POP����
//****************************************************************************************
#define CFG_FUNC_SW_DEBUG_EN

//****************************************************************************************
// ϵͳApp����ģʽѡ��
//****************************************************************************************
//#define CFG_APP_IDLE_MODE_EN
#define CFG_APP_BT_MODE_EN
//#define	CFG_APP_USB_PLAY_MODE_EN
//#define	CFG_APP_CARD_PLAY_MODE_EN
//#define	CFG_APP_LINEIN_MODE_EN
//#define CFG_APP_RADIOIN_MODE_EN
#define CFG_APP_USB_AUDIO_MODE_EN

//#define CFG_APP_I2SIN_MODE_EN
//#define CFG_FUNC_I2S_MIX_MODE

//#define	CFG_APP_OPTICAL_MODE_EN	// SPDIF ����ģʽ
//#define CFG_APP_COAXIAL_MODE_EN	// SPDIF ͬ��ģʽ
//#define CFG_APP_HDMIIN_MODE_EN //HDMI INģʽ

#define CFG_FUNC_OPEN_SLOW_DEVICE_TASK

#ifdef CFG_APP_LINEIN_MODE_EN
#if defined(CFG_CHIP_BP1064A2) || defined(CFG_CHIP_BP1064L2) || defined(CFG_CHIP_BP10128)
#define LINEIN_INPUT_CHANNEL				(ANA_INPUT_CH_LINEIN1)
#elif defined(CFG_CHIP_BP1032)
#define LINEIN_INPUT_CHANNEL				(ANA_INPUT_CH_LINEIN4)
#else
#define LINEIN_INPUT_CHANNEL				(ANA_INPUT_CH_LINEIN5)
#endif
#endif

#ifdef CFG_APP_RADIOIN_MODE_EN
    #define FUNC_RADIO_RDA5807_EN
    //#define FUNC_RADIO_QN8035_EN  //оƬioʱ��ֻ֧��12M��qn8035������Ҿ���
#if defined(FUNC_RADIO_RDA5807_EN) && defined(FUNC_RADIO_QN8035_EN)
   #error Conflict: radio type error //����ͬʱѡ��������ʾģʽ
#endif
#endif

/**USB��������������һ��ͨ���� **/
#define CFG_FUNC_USB_DEVICE_DETECT
#ifdef  CFG_APP_USB_AUDIO_MODE_EN
	#define CFG_PARA_USB_MODE	AUDIO_MIC
	#define CFG_RES_AUDIO_USB_IN_EN		//ȱʡ ��Ϊģʽͨ·ʹ��
	#define CFG_PARA_AUDIO_USB_IN_SYNC	//ʱ��ƫ������� ������ͬ��
	#define CFG_PARA_AUDIO_USB_IN_SRC	//ת����׼��

	#define CFG_RES_AUDIO_USB_OUT_EN
	#define CFG_PARA_AUDIO_USB_OUT_SYNC	//ʱ��ƫ������� ������ͬ��
	#define CFG_PARA_AUDIO_USB_OUT_SRC	//ת����׼��
	#define CFG_RES_AUDIO_USB_VOL_SET_EN
	//#define USB_READER_EN
#endif

//IDLEģʽ(�ٴ���),powerkey/deepsleep����ͬʱѡ��Ҳ���Ե�������
//ͨ����Ϣ���벻ͬ��ģʽ
//MSG_DEEPSLEEP/MSG_POWER/MSG_POWERDOWN --> ����IDLEģʽ(�ٴ���)
//MSG_DEEPSLEEP --> ����IDLEģʽ�Ժ����CFG_IDLE_MODE_DEEP_SLEEP�򿪽���deepsleep
//MSG_POWERDOWN --> ����IDLEģʽ�Ժ����CFG_IDLE_MODE_POWER_KEY�򿪽���powerdown
#ifdef  CFG_APP_IDLE_MODE_EN
	#define CFG_IDLE_MODE_POWER_KEY	//power key
	#define CFG_IDLE_MODE_DEEP_SLEEP //deepsleep
	#ifdef CFG_IDLE_MODE_POWER_KEY
		#define BAKEUP_FIRST_ENTER_POWERDOWN		//��һ���ϵ���Ҫ����PowerKey
		#define POWERKEY_MODE		POWERKEY_MODE_SLIDE_SWITCH_LPD//POWERKEY_MODE_PUSH_BUTTON
		#if (POWERKEY_MODE == POWERKEY_MODE_SLIDE_SWITCH_LPD) || (POWERKEY_MODE == POWERKEY_MODE_SLIDE_SWITCH_HPD)
			#define POWERKEY_CNT					20
		#else
			#define POWERKEY_CNT					2000
		#endif

		#if (POWERKEY_MODE == POWERKEY_MODE_PUSH_BUTTON)
			//powerkey���ö̰������ܡ�
			#define USE_POWERKEY_PUSH_BUTTON_MSG_SP		MSG_PLAY_PAUSE
		#endif	
	#endif
	#ifdef CFG_IDLE_MODE_DEEP_SLEEP
		/*���ⰴ������,ע��CFG_PARA_WAKEUP_GPIO_IR�� ���Ѽ�IR_KEY_POWER����*/
		#define CFG_PARA_WAKEUP_SOURCE_IR		SYSWAKEUP_SOURCE9_IR//�̶�source9
		/*ADCKey���� ���CFG_PARA_WAKEUP_GPIO_ADCKEY ���Ѽ�ADCKEYWAKEUP���ü����ƽ*/
		#define CFG_PARA_WAKEUP_SOURCE_ADCKEY	SYSWAKEUP_SOURCE1_GPIO//��ʹ��ADC���ѣ������CFG_RES_ADC_KEY_SCAN ��CFG_RES_ADC_KEY_USE
		#define CFG_PARA_WAKEUP_SOURCE_POWERKEY SYSWAKEUP_SOURCE6_POWERKEY
		#define CFG_PARA_WAKEUP_SOURCE_IOKEY1	SYSWAKEUP_SOURCE3_GPIO
		#define CFG_PARA_WAKEUP_SOURCE_IOKEY2	SYSWAKEUP_SOURCE4_GPIO
	#endif
#endif

#if  defined(CFG_CHIP_MT166) || defined(CFG_CHIP_BP1032A1) || defined(CFG_CHIP_BP1032A2)
	#define CFG_RES_MIC_SELECT      (1)//0=NO MIC, 1= MIC1, 2= MIC2, 3 = MCI1+MIC2
#else
	#define CFG_RES_MIC_SELECT      (3)//0=NO MIC, 1= MIC1, 2= MIC2, 3 = MCI1+MIC2
#endif

//****************************************************************************************
//                 ��Ƶ���ͨ���������
//˵��:
//    �������Դ��ͬʱ�����
//****************************************************************************************
/**DAC-0ͨ������ѡ��**/
//#define CFG_RES_AUDIO_DAC0_EN

/**DAC-Xͨ������ѡ��**/
//#define CFG_RES_AUDIO_DACX_EN

/**I2S��Ƶ���ͨ������ѡ��**/
#define CFG_RES_AUDIO_I2SOUT_EN

//****************************************************************************************
//     I2S�������ѡ��
//˵��:
//    1.I2S���Ҳʹ��ʱ�˿�ѡ���ģʽ��Ҫע�Ᵽ��һ��;
//	  2��ȱʡ����ΪMaster��I2S������Ƶʹ��Master��MCLK,����Է���΢��ʱ�Ӿͻ���pop����
//    3.��������I2S slave��I2S��������ṩ�ȶ�ʱ�ӣ�����������߹������ȶ���������AudioIOSet.Sync = FALSE����������DetectLock����Sync
//****************************************************************************************
#if defined(CFG_APP_I2SIN_MODE_EN) || defined(CFG_RES_AUDIO_I2SOUT_EN)
	#include"i2s.h"
	#define	CFG_RES_I2S 						1               //I2S0_MODULE:I2S0����   I2S1_MODULE:I2S1����;
	#define	CFG_RES_I2S_MODE					0     //0:master mode ;1:slave mode ����δ�Ӳ�Ҫ��slave
	#if (CFG_RES_I2S == 0)					                	//MCLK-LRCLK-BCLK-DOUT-DIN Port 2/3ȱ�˿ں͹��ܣ���������
		#define	CFG_RES_I2S_MODULE				I2S0_MODULE
		#define	CFG_RES_I2S_IO_PORT				0//  0:i2S0_0-A0-1-2-3-4; 		1:I2S0_1-A24-A20-21-22-23; 	2:I2S0_2-X-X-X-A4-3;	3:I2S0_3-X-X-X-A23-22 //2��3 ȱclk ������δ����
	#elif (CFG_RES_I2S == 1)
		#define	CFG_RES_I2S_MODULE				I2S1_MODULE
		#define	CFG_RES_I2S_IO_PORT				1                               //  0:i2S1_0-A27-28-29-30-31; 	1:I2S1_1-A7-8-9-10-11; 		2:I2S1_2-X-A1-2-31-30;	3:I2S1_3-X-A20-21-11-10
	#endif
	#define CFG_PARA_I2S_SAMPLERATE				44100
	#define CFG_FUNC_I2S_IN_SYNC_EN				//ȱʡΪSRA
	#define CFG_FUNC_I2S_OUT_SYNC_EN
#endif



#ifdef CFG_FUNC_I2S_MIX_MODE
	#define	CFG_RES_I2S0_EN	 // 0:i2S0_0-A0-1-2-3-4; 		1:I2S0_1-A24-A20-21-22-23;  2:I2S0_2-X-X-X-A4-3;	3:I2S0_3-X-X-X-A23-22 //2��3 ȱclk ������δ����
	#define	CFG_RES_I2S1_EN	 // 0:i2S1_0-A27-28-29-30-31; 	1:I2S1_1-A7-8-9-10-11; 	    2:I2S1_2-X-A1-2-31-30;	3:I2S1_3-X-A20-21-11-10
	
	#ifdef CFG_RES_I2S0_EN
	#define CFG_RES_AUDIO_I2S0IN_EN
	#define CFG_RES_AUDIO_I2S0OUT_EN
	//#define CFG_FUNC_I2S0IN_SRC_EN  			//I2S���������ת��
	//#define CFG_FUNC_I2S0IN_SRA_EN  			//I2S���������΢��,�ڲ�ʱ��,slaverʱ����
	//#define CFG_FUNC_I2S0OUT_SRC_EN  			//I2S���������ת��
	//#define CFG_FUNC_I2S0OUT_SRA_EN  			//I2S���������΢��,�ڲ�ʱ��,slaverʱ����
	//#define CFG_PARA_I2S0_SAMPLE  	44100 	//I2S�����ʣ�һ�㲻�ã��ɵ����ļ�����
    #define CFG_FUNC_I2S0_MIX_EN        0		// 0 = ������Ч����    	1 = ��music_pcm��Ϻ�һ������
    #define	CFG_RES_I2S0_IO_PORT		0		// 0:i2S0_0-A0-1-2-3-4; 	1:I2S0_1-A24-A20-21-22-23;  2:I2S0_2-X-X-X-A4-3;	3:I2S0_3-X-X-X-A23-22 //2��3 ȱclk ������δ����
	#endif
	
	#ifdef CFG_RES_I2S1_EN
	#define CFG_RES_AUDIO_I2S1IN_EN
	#define CFG_RES_AUDIO_I2S1OUT_EN
	//#define CFG_FUNC_I2S1IN_SRC_EN  			//I2S���������ת��
	//#define CFG_FUNC_I2S1IN_SRA_EN  			//I2S���������΢��,�ڲ�ʱ��,slaverʱ����
	//#define CFG_FUNC_I2S1OUT_SRC_EN  			//I2S���������ת��
	//#define CFG_FUNC_I2S1OUT_SRA_EN  			//I2S���������΢��,�ڲ�ʱ��,slaverʱ����
	//#define CFG_PARA_I2S1_SAMPLE  	44100 	//I2S�����ʣ�һ�㲻�ã��ɵ����ļ�����
    #define CFG_FUNC_I2S1_MIX_EN        0		// 0=������Ч����    1=��music_pcm��Ϻ�һ������
    #define	CFG_RES_I2S1_IO_PORT		1		// 0:i2S1_0-A27-28-29-30-31; 	1:I2S1_1-A7-8-9-10-11;  2:I2S1_2-X-A1-2-31-30;	3:I2S1_3-X-A20-21-11-10
	#endif

	#define CFG_PARA_I2S_SAMPLERATE				44100
	#define CFG_FUNC_I2S_IN_SYNC_EN				//ȱʡΪSRA
	#define CFG_FUNC_I2S_OUT_SYNC_EN	
#endif

//****************************************************************************************
//                 ����������ѡ��
//˵��:
//    ���½���������ѡ���Ӱ��code size;
//****************************************************************************************
#define USE_MP3_DECODER
#define USE_WMA_DECODER
#define USE_SBC_DECODER
#define USE_WAV_DECODER
//#define USE_DTS_DECODER
#define USE_FLAC_DECODER	//24bit 1.5Mbps������ʱ����Ҫ����DECODER_FIFO_SIZE_FOR_PLAYER ���fifo�����������룺FLAC_INPUT_BUFFER_CAPACITY
//#define USE_AAC_DECODER
//#define USE_AIF_DECODER
//#define USE_AMR_DECODER
#define USE_APE_DECODER

//****************************************************************************************
//                 ����Ч��������
//****************************************************************************************
//�ߵ������ڹ�������˵��:
//    1.�˹����ǻ���MIC OUT EQ�����ֶ����õģ���Ҫ�ڵ���������ʹ�ܴ�EQ��
//    2.Ĭ��f5��Ӧbass gain,f6��Ӧtreb gain,�������������޸Ĵ�EQ filter��Ŀ����Ҫ��Ӧ�޸�BassTrebAjust()�ж�Ӧ��ţ�
//EQģʽ��������˵��:
//    1.�˹����ǻ���MUSIC EQ�����ֶ����õģ���Ҫ�ڵ���������ʹ�ܴ�EQ��
//    2.����flat/classic/pop/jazz/pop/vocal boost֮��ͨ��KEY���л�   
#define CFG_FUNC_AUDIO_EFFECT_EN //����Чʹ�ܿ���
#ifdef CFG_FUNC_AUDIO_EFFECT_EN

    //#define CFG_FUNC_ECHO_DENOISE          //�������ٵ���delayʱ��������
 	//#define CFG_FUNC_MUSIC_EQ_MODE_EN     //Music EQģʽ��������
	#ifdef CFG_FUNC_MUSIC_EQ_MODE_EN	    
 		#define CFG_FUNC_EQMODE_FADIN_FADOUT_EN    //EQģʽ�л�ʱfade in/fade out��������ѡ��,����EQģʽ����POP��ʱ������� 		
    #endif
	#define CFG_FUNC_MUSIC_TREB_BASS_EN    //Music�ߵ������ڹ������� 
    //#define CFG_FUNC_SILENCE_AUTO_POWER_OFF_EN     //���ź��Զ��ػ����ܣ�
    #ifdef CFG_FUNC_SILENCE_AUTO_POWER_OFF_EN      
		#define  SILENCE_THRESHOLD                 120        //�����źż�����ޣ�С�����ֵ��Ϊ���ź�
		#define  SILENCE_POWER_OFF_DELAY_TIME      10*60*100     //���źŹػ���ʱʱ�䣬��λ��ms
    #endif

	#define CFG_FUNC_AUDIO_EFFECT_ONLINE_TUNING_EN//���ߵ���
	#ifdef CFG_FUNC_AUDIO_EFFECT_ONLINE_TUNING_EN

		//**��ƵSDK�汾��,��Ҫ�޸�**/
		#define  CFG_EFFECT_MAJOR_VERSION						(CFG_SDK_MAJOR_VERSION)
		#define  CFG_EFFECT_MINOR_VERSION						(CFG_SDK_MINOR_VERSION)
		#define  CFG_EFFECT_USER_VERSION						(CFG_SDK_PATCH_VERSION)
	
		/**���ߵ���Ӳ���ӿ���USB HID�ӿڣ�����UART�ӿ�*/
		/**����ʹ��USB HID�ӿڣ��շ�buf��512Byte*/
		/**UART�ӿ�ռ��2·DMA���շ�Buf��2k Byte*/
		/**ʹ��USB HID��Ϊ�����ӿڣ�DMA��Դ����*/
		#define  CFG_COMMUNICATION_BY_USB		// usb or uart

//		#define	 CFG_UART_COMMUNICATION_TX_PIN					GPIOA10
//		#define  CFG_UART_COMMUNICATION_TX_PIN_MUX_SEL			(3)
//		#define  CFG_UART_COMMUNICATION_RX_PIN					GPIOA9
//		#define  CFG_UART_COMMUNICATION_RX_PIN_MUX_SEL			(1)

		#define  CFG_COMMUNICATION_CRYPTO						(0)////����ͨѶ����=1 ����ͨѶ������=0
		#define  CFG_COMMUNICATION_PASSWORD                     0x11223344//////���ֽڵĳ�������
	#endif

	//ʹ��flash��õĵ�������
	//��Ч�����洢��flash�̶�������
	//#define CFG_EFFECT_PARAM_IN_FLASH_EN
	#ifdef CFG_EFFECT_PARAM_IN_FLASH_EN
		#define CFG_EFFECT_PARAM_IN_FLASH_MAX_NUM				(10)//֧��flash�������������flash�ռ����ʱ���鰴������
		#define	CFG_EFFECT_PARAM_IN_FLASH_ACTIVCE_NUM			(6)
		#ifdef CFG_FUNC_AUDIO_EFFECT_ONLINE_TUNING_EN
			#define CFG_EFFECT_PARAM_UPDATA_BY_ACPWORKBENCH			//�򿪸ú����ͨ��ACPWorkBench��������4156 Byte RAM������ʱ���RAM���ſ��Թر������
		#endif
	#endif

#endif

//****************************************************************************************
//     ��Ƶ������ò���
//****************************************************************************************
#define	CFG_PARA_SAMPLE_RATE				(44100)//(48000)//
#define CFG_BTHF_PARA_SAMPLE_RATE			(16000)//����HFPģʽ��ͳһ������Ϊ16K
#define	CFG_PARA_SAMPLES_PER_FRAME          (256)//(512)	//ϵͳĬ�ϵ�֡����С //ע:�ر�����Ч����ʱ,��������Ϊ256,��ͨ��sample���ò�������һ��
#define	CFG_BTHF_PARA_SAMPLES_PER_FRAME     (256)			//����ͨ��ģʽ��֡����С
#define CFG_PARA_MIN_SAMPLES_PER_FRAME		(256)//         //ϵͳ֡��Сֵ����֤mic delay��С,ע��:��Ϊ128ʱ tws����Ч��U��mips�������㡣
#define CFG_PARA_MAX_SAMPLES_PER_FRAME		(512)//(512)

#if (BT_AVRCP_VOLUME_SYNC == ENABLE) && defined(CFG_APP_BT_MODE_EN)
#define CFG_PARA_MAX_VOLUME_NUM		        (16)	//SDK 16 ������,���iphone�ֻ���������ͬ�����ܶ��ƣ�������16����һһ��Ӧ�ֻ�����������
#define CFG_PARA_SYS_VOLUME_DEFAULT			(12)	//SDKĬ������
#else
#define CFG_PARA_MAX_VOLUME_NUM		        (32)	//SDK 32 ������
#define CFG_PARA_SYS_VOLUME_DEFAULT			(25)	//SDKĬ������
#endif

//****************************************************************************************
//     ת��������ѡ��
//˵��:
//    1.ʹ�ܸú��ʾϵͳ�Ὣ����ͳһ�Ĳ����������Ĭ��ʹ��44.1KHz;
//    2.�˰汾Ĭ�ϴ򿪣�����ر�!!!!!!!!!!
//****************************************************************************************	
#define	CFG_FUNC_MIXER_SRC_EN

//     ������Ӳ��΢������ѡ��
//˵��:
//	     Ӳ��΢��ͬһʱ��ֻ��ʹ�ܿ���һ��΢����ʹϵͳAUPLLʱ�Ӹ�����Դ
//****************************************************************************************	
//#define	CFG_FUNC_FREQ_ADJUST
#ifdef CFG_FUNC_FREQ_ADJUST
	#define CFG_PARA_BT_FREQ_ADJUST		//Btplay ģʽ�����ڼ� Ӳ��΢������CFG_PARA_BT_SYNC�����
 	#define CFG_PARA_HFP_FREQ_ADJUST	//ͨ��ģʽ �����ڼ� Ӳ��΢��  ʹ������΢�������и��档 ��CFG_PARA_HFP_SYNC���
#endif

//****************************************************************************************
//                 ¼����������
//****************************************************************************************
//#define CFG_FUNC_RECORDER_EN     // �˹����ݲ����ã��������Ż�
#ifdef CFG_FUNC_RECORDER_EN
	#define CFG_FUNC_RECORD_SD_UDISK	//¼����SD������U��
	//#define CFG_FUNC_RECORD_FLASHFS 	//����ͬʱ���� CFG_FUNC_RECORD_SD_UDISK
//�ݲ�֧��flash
//#ifdef CFG_APP_FLASH_FATFS_PLAY_MODE_EN
//	//#define CFG_FUNC_RECORD_FLASHFATFS	//rec to extern fatfs
//#endif

	#ifdef CFG_FUNC_RECORD_SD_UDISK
		#define CFG_FUNC_RECORD_UDISK_FIRST				//U�̺Ϳ�ͬʱ����ʱ��¼���豸����ѡ��U�̣���������ѡ��¼����SD����
		#define CFG_PARA_RECORDS_FOLDER 		"REC"	//¼��¼U��ʱ��Ŀ¼�ļ��С�ע��ffpresearch_init ʹ�ûص����������ַ�����
		#define CFG_FUNC_RECORDS_MIN_TIME		1000	//��λms�������˺��С��������ȵ��Զ�ɾ����
		#define CFG_PARA_REC_MAX_FILE_NUM       256     //¼���ļ������Ŀ

		#define MEDIAPLAYER_SUPPORT_REC_FILE            // U�̻�TF��ģʽ�£��򿪴˹��ܣ���֧�ֲ���¼���ļ�������ֻ����¼���ط�ģʽ�²���¼���ļ�
        //#define AUTO_DEL_REC_FILE_FUNCTION            //¼���ļ��ﵽ��������Զ�ɾ��ȫ��¼���ļ��Ĺ���ѡ��
	#endif

	#define CFG_PARA_REC_GAIN		        (8191)	    //����¼������   8191:+6db;7284:+5db;6492:+4db;5786:+3db;5157:+2db;4596:+1db;4095:0db;

	#define DEL_REC_FILE_EN


	/*ע��flash�ռ䣬�����ͻ   middleware/flashfs/file.h FLASH_BASE*/
	#ifdef CFG_FUNC_RECORD_FLASHFS
		#define CFG_PARA_FLASHFS_FILE_NAME		"REC.MP3"//RECORDER_FILE_NAME
	#endif

	//N >= 2 ������128ϵͳ֡�Լ�����ЧMIPS�ϸߣ����ȼ�Ϊ3�ı�����̴������ݽ������Ƽ�ֵΪ 6�����ϵͳ֡��mips��ʱ���Ե�СN,��Լram��
	#define MEDIA_RECORDER_FIFO_N				6
	#define MEDIA_RECORDER_FIFO_LEN				(CFG_PARA_MAX_SAMPLES_PER_FRAME * MEDIA_RECORDER_CHANNEL * MEDIA_RECORDER_FIFO_N)
	//�������в�����¼�����ʿ�����Ҫ���������Բ��� ����FILE_WRITE_FIFO_LEN��
	#define MEDIA_RECORDER_CHANNEL				2
	#define MEDIA_RECORDER_BITRATE				96 //Kbps
	#define MEDIA_RECODER_IO_BLOCK_TIME			1000//ms
	//FIFO_Len=(����(96) / 8 * ����ʱ��ms(1000) �����ʵ�λKbps,��Ч���룩
	//����SDIOЭ�飬д����������250*2ms���� ���ܣ�ʵ�ⲿ��U�̴���785ms������д��������Ҫ���������fifo�ռ� ȷ������������ȵ�����(��ͬ��)��
	#define FILE_WRITE_FIFO_LEN					((((MEDIA_RECORDER_BITRATE / 8) * MEDIA_RECODER_IO_BLOCK_TIME ) / 512) * 512)//(����U��/Card�����������RAM��Դ��ѡ400~1500ms��������512����
#endif //CFG_FUNC_RECORDER_EN
//****************************************************************************************
//                 U�̻�SD��ģʽ��ع�������
//    
//****************************************************************************************
#if(defined(CFG_APP_USB_PLAY_MODE_EN) || defined(CFG_APP_CARD_PLAY_MODE_EN) || BT_AVRCP_SONG_TRACK_INFOR)
/**LRC��ʹ��� **/
//#define CFG_FUNC_LRC_EN			 	// LRC����ļ�����

/*------browser function------*/
//#define FUNC_BROWSER_PARALLEL_EN  		//browser Parallel
//#define FUNC_BROWSER_TREE_EN  			//browser tree
#if	defined(FUNC_BROWSER_PARALLEL_EN)||defined(FUNC_BROWSER_TREE_EN)
#define FUNCTION_FILE_SYSTEM_REENTRY
#if defined(FUNC_BROWSER_TREE_EN)||defined(FUNC_BROWSER_PARALLEL_EN)
#define GUI_ROW_CNT_MAX		5		//�����ʾ������
#else
#define GUI_ROW_CNT_MAX		1		//�����ʾ������
#endif
#endif
/*------browser function------*/

/**�ַ���������ת�� **/
/**Ŀǰ֧��Unicode     ==> Simplified Chinese (DBCS)**/
/**�ַ�ת������fatfs�ṩ������Ҫ�����ļ�ϵͳ**/
/**���֧��ת���������ԣ���Ҫ�޸�fatfs���ñ�**/
/**ע�⣺ ֧���ֿ⹦����Ҫʹ�� flash����>=2M��оƬ**/

//#define CFG_FUNC_STRING_CONVERT_EN	// ֧���ַ�����ת��

/**ȡ��AA55�ж�**/
/*fatfs�ڴ���ϵͳMBR��DBR������β�д˱�Ǽ�⣬Ϊ��߷Ǳ������̼����ԣ��ɿ�������, Ϊ��Ч������Ч�̣�����Ĭ�Ϲر�*/
//#define	CANCEL_COMMON_SIGNATURE_JUDGMENT
//#define FUNC_UPDATE_CONTROL   //�����������̿���(ͨ������ȷ������)
#endif

/**USB Host��⹦��**/
#if(defined(CFG_APP_USB_PLAY_MODE_EN))
#define CFG_RES_UDISK_USE
#define CFG_FUNC_UDISK_DETECT
#endif

/**USB Device��⹦��**/
#if (defined (CFG_APP_USB_AUDIO_MODE_EN)) || (defined(CFG_COMMUNICATION_BY_USB))
	#define CFG_FUNC_USB_DEVICE_EN
#endif


//****************************************************************************************
//                 BT��������
//˵��:
//    1.������ع�������ϸ������bt_config.h�ж���!!!!!!!!!!
//    
//****************************************************************************************
#include "bt_config.h"
#ifdef BT_TWS_SUPPORT
// sync device
#ifdef	CFG_RES_AUDIO_DAC0_EN
	#define TWS_DAC0_OUT	1
#endif
#ifdef CFG_RES_AUDIO_DACX_EN
	#define TWS_DACX_OUT	2
#endif
#ifdef CFG_RES_AUDIO_I2SOUT_EN
	#if (CFG_RES_I2S == 0)
		#define TWS_IIS0_OUT	3
	#else
		#define TWS_IIS1_OUT	4
	#endif
#endif

	#define TWS_SINK_DEV_FRAME			(CFG_PARA_MAX_SAMPLES_PER_FRAME)
	#define TWS_SINK_DEV_FIFO_SAMPLES	(TWS_SINK_DEV_FRAME * 2)
//Key device for sync
	#define TWS_AUDIO_OUT_PATH	TWS_DAC0_OUT//TWS_IIS0_OUT//TWS_IIS1_OUT

#else
	#define TWS_AUDIO_OUT_PATH	0xff//�ͷ���Ƶ�ڴ�
#endif



//****************************************************************************************
//****************************************************************************************
/**OS����ϵͳ����IDLEʱ��core��������״̬���Դﵽ���͹���Ŀ��**/
/*ע�⣬����OS���ȵ�IDLE������Ӧ�ò�APPMODE��Ӧ�ò��������*/
#define CFG_FUNC_IDLE_TASK_LOW_POWER
#ifdef	CFG_FUNC_IDLE_TASK_LOW_POWER
	#define	CFG_GOTO_SLEEP_USE
#endif

//****************************************************************************************
//                 SHELL��������
//˵��:
//    1.shell���������Լ������뵽shell.c������;
//    2.shell����Ĭ�Ϲرգ�Ĭ��ʹ��UART1;
//****************************************************************************************
//#define CFG_FUNC_SHELL_EN

//****************************************************************************************
//                 UART DEBUG��������
//ע�⣺DEBUG�򿪺󣬻�����micͨ·��delay������ҪDEBUG���Դ���ʱ������رյ���
//****************************************************************************************
#define CFG_FUNC_DEBUG_EN
#ifdef CFG_FUNC_DEBUG_EN
	//#define CFG_USE_SW_UART
	#ifdef CFG_USE_SW_UART
		#define SW_UART_IO_PORT				    SWUART_GPIO_PORT_A//SWUART_GPIO_PORT_B
		#define SW_UART_IO_PORT_PIN_INDEX	    1//bit num
		#define  CFG_SW_UART_BANDRATE   		512000//software uart baud rate select:38400 57600 115200 256000 512000 1000000 ,default 512000
	#else
		#define CFG_UART_TX_PORT 				(0)//tx port  0--A6��1--A10, 2--A25, 3--A0, 4--A1
		#define  CFG_UART_BANDRATE   			512000//hardware uart baud set
	#endif
#endif

//****************************************************************************************
//                ��ʾ����������
//˵��:
//    1.��¼���߲μ�MVs26_SDK\tools\script��
//    2.��ʾ�����ܿ�����ע��flash��const data��ʾ��������ҪԤ����¼�����򲻻Ქ��;
//    3.const data���ݿ�����飬Ӱ�쿪���ٶȣ���Ҫ������֤��
//****************************************************************************************
#define CFG_FUNC_REMIND_SOUND_EN
#ifdef CFG_FUNC_REMIND_SOUND_EN
	#define CFG_PARAM_FIXED_REMIND_VOL   	10		//�̶���ʾ������ֵ,0��ʾ��music volͬ������
#endif

//****************************************************************************************
//                 �ϵ���书������        
//****************************************************************************************
#define CFG_FUNC_BREAKPOINT_EN
#ifdef CFG_FUNC_BREAKPOINT_EN
	#define BP_PART_SAVE_TO_NVM			// �ϵ���Ϣ���浽NVM
	#define BP_SAVE_TO_FLASH			// �ϵ���Ϣ���浽Flash
	#define FUNC_MATCH_PLAYER_BP		// ��ȡFSɨ����벥��ģʽ�ϵ���Ϣƥ����ļ����ļ��к�ID��
#endif

//****************************************************************************************
//                 Key �����������
//****************************************************************************************
/**����beep������**/
//#define  CFG_FUNC_BEEP_EN
    #define CFG_PARA_BEEP_DEFAULT_VOLUME    15//ע��:����������ͬ�����ܿ����󣬴�ֵ���Ϊ16

/**����˫������**/
#define  CFG_FUNC_DBCLICK_MSG_EN
#ifdef CFG_FUNC_DBCLICK_MSG_EN
	#define  CFG_PARA_CLICK_MSG             MSG_PLAY_PAUSE //����ִ����Ϣ
	#define  CFG_PARA_DBCLICK_MSG           MSG_BT_HF_REDAIL_LAST_NUM   //˫��ִ����Ϣ
	#define  CFG_PARA_DBCLICK_DLY_TIME      35       //˫����Ч���ʱ��:4ms*20=80ms
#endif

/**ADC����**/
#define CFG_RES_ADC_KEY_SCAN				//��device service ������Keyɨ��ADCKEY
#if defined(CFG_RES_ADC_KEY_SCAN) || defined(CFG_PARA_WAKEUP_SOURCE_ADCKEY)
	#define CFG_RES_ADC_KEY_USE				//ADC�������� ����
#endif

/**IR����**/
#define CFG_RES_IR_KEY_SCAN				//����device service Keyɨ��IRKey

/**������ť����**/
//#define	CFG_RES_CODE_KEY_USE

/**GPIO����**/
//#define CFG_RES_IO_KEY_SCAN	

/**��λ������ѡ��**/
//#define CFG_ADC_LEVEL_KEY_EN 

//***************************************************************************************
//					RTC/���ӹ�������
//ע��:
//   1.RTCʱ��Դѡ��32K���񷽰���ֻ��BP1064L2�ͺ�֧��;
//   2.����оƬ�ͺţ�RTCʱ��ԴĬ��ѡ��rcʱ�ӣ����ȵͣ���
//   3.����оƬ�ͺţ��Ƽ�12M����ģʽ��deepsleep��������̧��
//	 4.��RTC����
//		a.��Ҫ�ر�TWSģʽ,��CFG_FUNC_RTC_EN��
//		b.�ֶ�ɾ��BT_Audio_APP\middleware\bluetooth�ļ�����libtws.a�⣬�����ڹ���������ȥ��tws�������
//***************************************************************************************
#ifndef BT_TWS_SUPPORT
#ifdef CFG_CHIP_BP1064L2
	#define CFG_FUNC_RTC_EN
#else
	#ifndef CFG_FUNC_BACKUP_EN
		//#define CFG_FUNC_RTC_EN
	#endif
#endif

#ifdef CFG_FUNC_RTC_EN
	#define CFG_RES_RTC_EN
	#ifdef CFG_RES_RTC_EN
		#define CFG_PARA_RTC_12M 	//Ԥ�����ò���
	#endif
	
	#define CFG_FUNC_ALARM_EN  //���ӹ���,���뿪ʱ��
	#define CFG_FUNC_LUNAR_EN  //������,���뿪ʱ��
	#ifdef CFG_FUNC_ALARM_EN
		#define CFG_FUNC_SNOOZE_EN //����̰˯����
	#endif
#endif
#endif

//****************************************************************************************
//                Display ��ʾ����
//****************************************************************************************
//#define CFG_FUNC_DISPLAY_TASK_EN	//display task
//#define CFG_FUNC_DISPLAY_EN
#ifdef CFG_FUNC_DISPLAY_EN
//  #define  DISP_DEV_SLED
  #define  DISP_DEV_7_LED
/**8��LED��ʾ����**/
/*LED�Դ�ˢ����Ҫ��Timer1ms�жϽ��У���дflash����ʱ��ر��ж�*/
/*������Ҫ�����⴦�������ע�ú�����Ĵ����*/
/*ע��timer�жϷ������͵��õ���API�������TCM�������õ�����api���⺯������ѯ֧��*/
/*�����˺꣬Ҫ��ע����ʹ��NVIC_SetPriority ����Ϊ0�Ĵ��룬�����Ӧ�жϵ��÷�TCM��������������λ*/
#ifdef DISP_DEV_7_LED
  #define	CFG_FUNC_LED_REFRESH
#endif

#if defined(DISP_DEV_SLED) && defined(DISP_DEV_7_LED) && defined(LED_IO_TOGGLE)
   #error Conflict: display setting error //����ͬʱѡ��������ʾģʽ
#endif
#endif

//****************************************************************************************
//				   ������μ�⹦��ѡ������
//****************************************************************************************
//#define  CFG_FUNC_DETECT_PHONE_EN                            

//****************************************************************************************
//				   3�ߣ�4�߶������ͼ�⹦��ѡ������
//****************************************************************************************
//#define  CFG_FUNC_DETECT_MIC_SEG_EN  

//****************************************************************************************
//                            ���ó�ͻ ���뾯��
//****************************************************************************************

#if defined(CFG_FUNC_SHELL_EN) && defined(CFG_USE_SW_UART)
#error	Conflict: shell  X  SW UART No RX!
#endif

// ʹ���Զ�������
//#define AUTO_TEST_ENABLE

// ʹ������OTA
// OTA���Զ���������SPP
//#define  CFG_FUNC_BT_OTA_EN

#include "sys_gpio.h"


//************************************************************************************************************
//dump����,���Խ����ݷ��͵�dump����,���ڷ���
//************************************************************************************************************

//#define CFG_DUMP_DEBUG_EN
#ifdef CFG_DUMP_DEBUG_EN

//UART����,��Ҫ�ʹ�����־��ӡΪ��ͬ��UART��
//UART0: A0/A1/A6
//UART1: A10/A25/A19
	#define CFG_DUMP_UART_TX_PORT 				(4)//tx port  0--A6��1--A10, 2--A25, 3--A0, 4--A1, 5--A19
#if ((CFG_DUMP_UART_TX_PORT == 1) || (CFG_DUMP_UART_TX_PORT == 2) || (CFG_DUMP_UART_TX_PORT == 5))
	#define CFG_DUMP_UART_TX_PORT_GROUP			(1)
#else
	#define CFG_DUMP_UART_TX_PORT_GROUP			(0)
#endif
	#define CFG_DUMP_UART_BANDRATE 				(2000000)

	#define DUMP_UART_MODE_DAC					(0)
	#define DUMP_UART_MODE_LININ				(1)
	#define DUMP_UART_MODE_AEC					(2)
	#define DUMP_UART_MODE_SBC					(3)
	#define DUMP_UART_MODE_SCO					(4)

	#define CFG_DUMP_UART_MODE					(DUMP_UART_MODE_DAC)

	//���ó�ͻ ���뾯��
	//DUMP_DEBUG �� UART DEBUG ����ʹ��ͬһ�鴮��
#ifdef CFG_FUNC_DEBUG_EN
	#if(((CFG_UART_TX_PORT == 1) || (CFG_UART_TX_PORT == 2) || (CFG_UART_TX_PORT == 5)) && ((CFG_DUMP_UART_TX_PORT == 1) || (CFG_DUMP_UART_TX_PORT == 2) || (CFG_DUMP_UART_TX_PORT == 5)))
		#error "Different ports are required !!!"
	#endif
	#if(((CFG_UART_TX_PORT == 0) || (CFG_UART_TX_PORT == 3) || (CFG_UART_TX_PORT == 4)) && ((CFG_DUMP_UART_TX_PORT == 0) || (CFG_DUMP_UART_TX_PORT == 3) || (CFG_DUMP_UART_TX_PORT == 4)))
		#error "Different ports are required !!!"
	#endif
#endif

	//a19 ��usb�ӿڵ�dp��ʹ�ô�io��Ҫ�ر�����usb����
#if(CFG_DUMP_UART_TX_PORT == 5)
	#ifdef CFG_APP_USB_PLAY_MODE_EN
		#error "Need Close CFG_APP_USB_PLAY_MODE_EN !!!"
	#endif
	#ifdef CFG_APP_USB_AUDIO_MODE_EN
		#error "Need Close CFG_APP_USB_AUDIO_MODE_EN !!!"
	#endif
	#ifdef CFG_COMMUNICATION_BY_USB
		#error "Need Close CFG_COMMUNICATION_BY_USB !!!"
	#endif
#endif

#endif /* CFG_DUMP_DEBUG_EN */


#endif /* APP_CONFIG_H_ */
