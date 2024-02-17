#include <stdlib.h>
#include <nds32_intrinsic.h>
#include "uarts.h"
#include "uarts_interface.h"
#include "backup.h"
#include "backup_interface.h"
#include "type.h"
#include "debug.h"
#include "timeout.h"
#include "clk.h"
#include "dma.h"
#include "timer.h"
#include "adc.h"
#include "i2s.h"
#include "watchdog.h"
#include "reset.h"
#include "rtc.h"
#include "spi_flash.h"
#include "gpio.h"
#include "chip_info.h"
#include "irqn.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "remap.h"
#include "rtos_api.h"
#include "main_task.h"
#include "dac.h" //for volscale
#include "otg_detect.h"
#include "sw_uart.h"
#include "remind_sound.h"
#ifdef CFG_APP_BT_MODE_EN
#include "bt_common_api.h"
#endif
#include "file.h"
#include "flash_boot.h"
#include "sadc_interface.h"
#include "app_config.h"
#include "powercontroller.h"
#include "audio_decoder_api.h"
#include "sys.h"
#ifdef CFG_FUNC_DISPLAY_EN
#include "display.h"
#endif

#ifdef CFG_APP_BT_MODE_EN
#if (BT_HFP_SUPPORT == ENABLE)
#include "bt_hf_mode.h"
#endif
#endif
#include "tws_mode.h"
#include "rtc_timer.h"
#include "rtc_ctrl.h"
#include "efuse.h"
#include "mode_task.h"
#include "device_detect.h"
#include "idle_mode.h"
#include "flash_table.h"
#include "sys_param.h"

#include "bt_em_config.h"


//-----------------globle timer----------------------//
volatile uint32_t gInsertEventDelayActTimer = 2500; // ms
volatile uint32_t gChangeModeTimeOutTimer = CHANGE_MODE_TIMEOUT_COUNT;
volatile uint32_t gDeviceCheckTimer = DEVICE_DETECT_TIMER; //ms
volatile uint32_t gDeviceUSBDeviceTimer = DEVICE_USB_DEVICE_DETECT_TIMER ;//ms
#ifdef CFG_FUNC_CARD_DETECT	
volatile uint32_t gDeviceCardTimer = DEVICE_CARD_DETECT_TIMER ;//ms
#endif
#ifdef CFG_LINEIN_DET_EN
volatile uint32_t gDeviceLineTimer = DEVICE_LINEIN_DETECT_TIMER ;//ms
#endif

#ifdef HDMI_HPD_CHECK_DETECT_EN
volatile uint32_t gDevicehdmiTimer = DEVICE_HDMI_DETECT_TIMER ;//ms
#endif

#ifdef CFG_FUNC_BREAKPOINT_EN
volatile uint32_t gBreakPointTimer = 0 ;//ms
#endif
#if defined(CFG_APP_IDLE_MODE_EN)&&defined(CFG_FUNC_REMIND_SOUND_EN)
volatile uint32_t gIdleRemindSoundTimeOutTimer = 0 ;//ms
#endif
//-----------------globle timer----------------------//
extern void DBUS_Access_Area_Init(uint32_t start_addr);
extern const unsigned char *GetLibVersionFatfsACC(void);
extern void UsbAudioTimer1msProcess(void);
extern void user_fs_api_init(void);
extern void EnableSwUartAsUART(uint8_t EnableFlag); //retarget.c
extern void report_up_grate(void);
extern void vApplicationIdleHook(void);
extern void trace_TASK_SWITCHED_IN(void);
extern void trace_TASK_SWITCHED_OUT(void);
extern void bt_api_init(void);
extern void uart_log_out(void);
typedef void (*rtosfun)(void);

#ifndef CFG_FUNC_STRING_CONVERT_EN
extern rtosfun pvApplicationIdleHook;
extern rtosfun ptrace_TASK_SWITCHED_IN;
extern rtosfun ptrace_TASK_SWITCHED_OUT;
#endif

extern volatile uint8_t uart_switch;


void _printf_float()
{

}


void ResetFlagGet(uint8_t Flag)
{
	APP_DBG("RstFlag = %x\n", Flag);

	if(Flag & 0x01)
	{
		APP_DBG("power on reset\n");
	}
	if(Flag & 0x02)
	{
		APP_DBG("pin reset\n");
	}
	if(Flag & 0x04)
	{
		APP_DBG("watchdog reset\n");
	}
	if(Flag & 0x08)
	{
		APP_DBG("LVD reset\n");
	}
	if(Flag & 0x10)
	{
		APP_DBG("cpu debug reset\n");
	}
	if(Flag & 0x20)
	{
		APP_DBG("system reset\n");
	}	
	if(Flag & 0x40)
	{
		APP_DBG("cpu core reset\n");
	}
	APP_DBG("\n");
}

//__attribute__((section(".driver.isr")))

void OneMSTimer(void)
{
	if(gInsertEventDelayActTimer)gInsertEventDelayActTimer--;
	if(gChangeModeTimeOutTimer)gChangeModeTimeOutTimer--;
	if(gDeviceCheckTimer)gDeviceCheckTimer--;
#ifdef CFG_FUNC_CARD_DETECT	
	if(gDeviceCardTimer > 1)gDeviceCardTimer--;
#endif
#ifdef CFG_LINEIN_DET_EN
	if(gDeviceLineTimer > 1)gDeviceLineTimer--;
#endif
#ifdef HDMI_HPD_CHECK_DETECT_EN
	if(gDevicehdmiTimer > 1)gDevicehdmiTimer--;
#endif
	if(gDeviceUSBDeviceTimer > 1)gDeviceUSBDeviceTimer--;
#ifdef CFG_FUNC_BREAKPOINT_EN	
	if(gBreakPointTimer > 1)gBreakPointTimer--;
#endif
#if defined(CFG_APP_IDLE_MODE_EN)&&defined(CFG_FUNC_REMIND_SOUND_EN)
	gIdleRemindSoundTimeOutTimer++;
#endif

}

#if defined(BT_TWS_SUPPORT) && (TWS_PAIRING_MODE == CFG_TWS_ROLE_RANDOM)
uint32_t doubleKeyCnt = 0;
#endif
void Timer2Interrupt(void)
{
	Timer_InterruptFlagClear(TIMER2, UPDATE_INTERRUPT_SRC);
#if defined(CFG_FUNC_USB_DEVICE_EN) || defined(CFG_FUNC_UDISK_DETECT)
	OTG_PortLinkCheck();
#endif

#if defined(BT_TWS_SUPPORT) && (TWS_PAIRING_MODE == CFG_TWS_ROLE_RANDOM)
	if(doubleKeyCnt>1)
	{
		doubleKeyCnt--;
	}
#endif

#ifdef CFG_APP_USB_AUDIO_MODE_EN
	UsbAudioTimer1msProcess(); //1ms�жϼ��
#endif

#ifdef CFG_APP_BT_MODE_EN
#if (BT_HFP_SUPPORT == ENABLE)
	BtHf_Timer1msProcess();
#endif
#endif
	uart_log_out();
	OneMSTimer();
}

#ifdef CFG_FUNC_LED_REFRESH
__attribute__((section(".tcm_section")))
void Timer6Interrupt(void)
{
	Timer_InterruptFlagClear(TIMER6, UPDATE_INTERRUPT_SRC);

	//ʾ�����룬��Ҫ��ӶΡ�.tcm_section��
	//�ؼ���    __attribute__((section(".tcm_section")))
	//�ͻ���Ҫ���Լ���ʵ�ֵ�API������ӹؼ���
	//GPIO_RegOneBitSet(GPIO_A_TGL, GPIO_INDEX2);
	extern void LedFlushDisp(void);
	LedFlushDisp();
}
#endif

void SystemClockInit(void)
{
	//clock����
	Clock_Config(1, 24000000);
	#ifdef BT_TWS_SUPPORT
	//�˴���Ƶ�����޸�
	Clock_PllLock(293530);//owen 11.2896*26=293.5296MHz
	*(uint32_t*)0x40026008 = 0x24B0F2;//��������BUG����8λ��������

	#else
	Clock_PllLock(288000);
	#endif
	Clock_APllLock(240000);
	Clock_USBClkDivSet(4);// bkd add for u disk 60M 2019.4.17
	Clock_SysClkSelect(PLL_CLK_MODE);
	Clock_USBClkSelect(APLL_CLK_MODE);
	Clock_UARTClkSelect(APLL_CLK_MODE);
	Clock_Timer3ClkSelect(RC_CLK_MODE);//for cec rc clk

	Clock_Module1Enable(ALL_MODULE1_CLK_SWITCH);
	Clock_Module2Enable(ALL_MODULE2_CLK_SWITCH);
	Clock_Module3Enable(ALL_MODULE3_CLK_SWITCH);
	Clock_ApbClkDivSet(5);
}

void LogUartConfig(bool InitBandRate)
{
#ifdef CFG_FUNC_DEBUG_EN
#ifdef CFG_USE_SW_UART
	#if (SW_UART_IO_PORT == GPIO_A_IN)	
		GPIO_PortAModeSet(1<<SW_UART_IO_PORT_PIN_INDEX, 0);
	#else
		GPIO_PortBModeSet(1<<SW_UART_IO_PORT_PIN_INDEX, 0);
	#endif
	SwUartTxInit(SW_UART_IO_PORT, SW_UART_IO_PORT_PIN_INDEX, CFG_SW_UART_BANDRATE);
	if(InitBandRate)
	{
		EnableSwUartAsUART(1);
	}
#else
	switch (CFG_UART_TX_PORT)
	{
	case 0:
#ifdef CFG_FUNC_SHELL_EN
		GPIO_PortAModeSet(GPIOA5, 1);//Rx, A5:uart0_rxd_1
#endif
		GPIO_PortAModeSet(GPIOA6, 5);//Tx, A6:uart0_rxd_1
		if(InitBandRate)
		{
			DbgUartInit(0, CFG_UART_BANDRATE, 8, 0, 1);
		}
		break;
	case 1:
#ifdef CFG_FUNC_SHELL_EN
		GPIO_PortAModeSet(GPIOA9, 1);//Rx, A9:uart1_rxd_0
#endif
		GPIO_PortAModeSet(GPIOA10, 3);//Tx,A10:uart1_txd_0
		if(InitBandRate)
		{
			DbgUartInit(1, CFG_UART_BANDRATE, 8, 0, 1);
		}
		break;
	case 2:
#ifdef CFG_FUNC_SHELL_EN
		GPIO_PortAModeSet(GPIOA24, 1);	//Rx, A24:uart1_rxd_0
#endif
		GPIO_PortAModeSet(GPIOA25, 6);	//Tx, A25:uart1_txd_0
		if(InitBandRate)
		{
			DbgUartInit(1, CFG_UART_BANDRATE, 8, 0, 1);
		}
		break;
	case 3:
#ifdef CFG_FUNC_SHELL_EN
		GPIO_PortAModeSet(GPIOA1, 0x02);
#endif
		GPIO_PortAModeSet(GPIOA0, 0x08);	//Tx, A0:1000:uart0_txd_2(o)
		if(InitBandRate)
		{
			DbgUartInit(0, CFG_UART_BANDRATE, 8, 0, 1);
		}
		break;
	case 4:
#ifdef CFG_FUNC_SHELL_EN
		GPIO_PortAModeSet(GPIOA0, 0x02);
#endif
		GPIO_PortAModeSet(GPIOA1, 0x0c);	//Tx, A1:1100:uart0_txd_0(o)
		if(InitBandRate)
		{
			DbgUartInit(0, CFG_UART_BANDRATE, 8, 0, 1);
		}
		break;
	default:
		break;
	}
#endif
#else
	DbgUartInit(1, 0, 8, 0, 1);
#endif
}

#ifdef CFG_IDLE_MODE_DEEP_SLEEP
extern void FshcClkSwitch(FSHC_CLK_MODE ClkSrc, uint32_t flash_clk);
void SleepMain(void)
{
#ifdef BT_TWS_SUPPORT
	RTC_IntDisable();
	NVIC_DisableIRQ(Rtc_IRQn);
	RTC_ClockSrcSel(RC_32K);
#endif
	Power_DeepSleepLDO12ConfigTest(0,5,0);//����deepsleep��1V2 ����Ϊ1.0V���Ը���ʡ�硣
	Clock_UARTClkSelect(RC_CLK_MODE);//���л�log clk������������ٴ���
	LogUartConfig(TRUE); //scan����ӡʱ ������
	SysTickDeInit();
	Efuse_ReadDataDisable(); //////////�ر�EFUSE////////
	SpiFlashInit(80000000, MODE_1BIT, 0, FSHC_PLL_CLK_MODE);//rcʱ�� ��֧��flash 4bit��ϵͳ�ָ�ʱ���䡣
	FshcClkSwitch(FSHC_RC_CLK_MODE, 80000000);//����RC
	Clock_DeepSleepSysClkSelect(RC_CLK_MODE, FSHC_RC_CLK_MODE, 1);
	Clock_PllClose();
	Clock_AUPllClose();//AUPLL������Լ980uA�Ĺ���
#if !defined(CFG_RES_RTC_EN)
	Clock_HOSCDisable();//����RTCӦ�ò���RTC����ʱ����HOSC���򲻹ر�HOSC����24M����
#endif
//	Clock_LOSCDisable(); //����RTCӦ�ò���RTC����ʱ����LOSC���򲻹ر�LOSC����32K����

	SysTickInit();
}

void WakeupMain(void)
{
	Chip_Init(1);
	SysTickDeInit();
	WDG_Enable(WDG_STEP_4S);
	SystemClockInit();
	SysTickInit();
	Efuse_ReadDataEnable(); 
	Clock_DeepSleepSysClkSelect(PLL_CLK_MODE, FSHC_PLL_CLK_MODE, 0);
	SpiFlashInit(96000000, MODE_4BIT, 0, FSHC_PLL_CLK_MODE);
	LogUartConfig(TRUE);//����ʱ�Ӻ����䴮��ǰ��Ҫ��ӡ��
	//Clock_Pll5ClkDivSet(8);// 
	//B0B1ΪSW��ʽ�˿ڣ��ڵ��Խ׶���ϵͳ�����˵͹���ģʽʱ�ر���GPIO����ģʽ�����ڴ˴�����
	//GPIO_PortBModeSet(GPIOB0, 0x3); //B0 sw_clk(i) 
	//GPIO_PortBModeSet(GPIOB1, 0x4); //B1 sw_d(io)
	//APP_DBG("Main:wakeup\n");
}
#endif


extern const RTC_DATE_TIME ResetRtcVal;
#ifdef CFG_APP_BT_MODE_EN
extern uint32_t bt_em_size(void);
void bt_em_size_init(void)
{
	uint32_t bt_em_mem;
	app_bt_em_params_config();

	bt_em_mem = bt_em_size();
//	APP_DBG("bt em size:%x\n", bt_em_mem);
	if(bt_em_mem%4096)
	{
		bt_em_mem = ((bt_em_mem/4096)+1)*4096;
	}
	if(bt_em_mem > BB_EM_SIZE)
	{
		APP_DBG("bt em config error!\nyou must check BB_EM_SIZE\n%s%u \n",__FILE__,__LINE__);
		while(1);
	}
	else
	{
		APP_DBG("bt em size:%uKB\n", (unsigned int)bt_em_mem/1024);
	}
}
#endif


int main(void)
{
	uint16_t RstFlag = 0;
	extern char __sdk_code_start;
	
	Chip_Init(1);
	WDG_Enable(WDG_STEP_4S);
	//WDG_Disable();

	//ע�⣺
	//����г�������Ӱ�쾧�����Ƶ�ʣ�Ĭ��ֵΪ9������ֵ��Լ0.72mA�����ֵ15������ֵԼ1.152mA
	//��ͬ�ľ���г����ݣ�����ֵԽ�󣬾���ڷ�Խ�󣬹���ҲԽ��
	//����ֵһ���趨֮���ܶ�̬�޸ģ���Ӱ�������Ĺ���
	//Clock_HOSCCurrentSet(13);//����г���������Ϊ15������1.152mA��
	
//#if FLASH_BOOT_EN
//	RstFlag = Reset_FlagGet_Flash_Boot();
//#else
	RstFlag = Reset_FlagGet();
	Reset_FlagClear();
//#endif

	//�����Ҫʹ��NVM�ڴ�ʱ����Ҫ���ø�API����һ��ϵͳ�ϵ���Ҫ�����NVM�ڴ������������breakpoint ��ʵʩ��
	BACKUP_NVMInit();

	Power_LDO12Config(1250);	//ʹ��320M��Ƶʱ��Ҫ������1.25v��ʹ��288M��������ε�����

	SystemClockInit();

	LogUartConfig(TRUE);
#ifdef CFG_DUMP_DEBUG_EN
	DumpUartConfig(TRUE);
#endif

	DBUS_Access_Area_Init(0);//����Databus��������Ϊcodesize
	Remap_DisableTcm();
	Remap_InitTcm(FLASH_ADDR, 8);//flash boot(common code)ռ�õ�TCM�ռ�8K

#ifdef FUNC_OS_DEBUG //�����ڵ���ģʽ
	memcpy((void *)0x20000000, (void *)(0x40000), 20*1024);
	Remap_AddrRemapSet(ADDR_REMAP1, 0x40000, 0x20000000, 20);//SDKռ�õ�TCM�ռ�20K
#else
	memcpy((void *)0x20006000, (void *)(&__sdk_code_start), TCM_SIZE*1024);
	Remap_AddrRemapSet(ADDR_REMAP1, (uint32_t)(&__sdk_code_start), 0x20006000, TCM_SIZE);//SDKռ�õ�TCM�ռ�12K
#endif
	SpiFlashInit(96000000, MODE_4BIT, 0, FSHC_PLL_CLK_MODE);
	Clock_RC32KClkDivSet(Clock_RcFreqGet(TRUE) / 32000);//��������
	
	//���ǵ���������8M flash��д֮ǰ��ҪUnlock��SDKĬ�ϲ�����������
	//�û�Ϊ������flash ��ȫ�ԣ�������Լ�flash��С��ʵ�����������flash������������
	//SpiFlashIOCtrl(IOCTL_FLASH_PROTECT, FLASH_LOCK_RANGE_HALF);//��������code����

	prvInitialiseHeap();
	osSemaphoreMutexCreate();//Ӳ������OS������������������ڴ�����֮�����log��������������Ҫ��ʼ����ջ�󴴽������ģ�⴮�ڲ�Ӱ�졣
	
#ifdef CFG_RES_RTC_EN
	#ifdef CFG_CHIP_BP1064L2
    RTC_ClockSrcSel(OSC_32K);//�˺����Ĳ���ѡ����������ϵͳ��ʼ��ѡ��ľ��񣨡�Clock_Config()��������һ��
	#else
	RTC_ClockSrcSel(OSC_24M);
	#endif
	
	RTC_IntDisable();//Ĭ�Ϲر�RTC�ж�
	RTC_IntFlagClear();
	RTC_WakeupDisable();	
		
#endif

#ifdef CFG_SOFT_POWER_KEY_EN
	SoftPowerInit();
	WaitSoftKey();
#endif

	NVIC_EnableIRQ(SWI_IRQn);
	GIE_ENABLE();	//�������ж�

#ifdef CFG_FUNC_LED_REFRESH
	//Ĭ�����ȼ�Ϊ0��ּ�����ˢ�����ʣ��ر��Ƕϵ�����дflash������Ӱ��ˢ���������ϸ���������timer6�жϵ��ö���TCM���룬�����õ�driver�����
	//��ȷ��GPIO_RegOneBitSet��GPIO_RegOneBitClear��TCM��������api����ȷ�ϡ�
	NVIC_SetPriority(Timer6_IRQn, 0);
 	Timer_Config(TIMER6,1000,0);
 	Timer_Start(TIMER6);
 	NVIC_EnableIRQ(Timer6_IRQn);

 	//���д������������ʱ�����Timer�жϴ��������ͻ�һ��Ҫ���޸ĵ���
 	//GPIO_RegOneBitSet(GPIO_A_OE, GPIO_INDEX2);//only test��user must modify
#endif

#ifdef CFG_FUNC_DISPLAY_EN
 	DispInit(0);
#endif

	APP_DBG("\n");
	APP_DBG("****************************************************************\n");
	APP_DBG("|                    MVsilicon B1 SDK                          |\n");
	APP_DBG("|            Mountain View Silicon Technology Co.,Ltd.         |\n");
	APP_DBG("|            SDK Version: %d.%d.%d                                |\n", CFG_SDK_MAJOR_VERSION, CFG_SDK_MINOR_VERSION, CFG_SDK_PATCH_VERSION);
	APP_DBG("****************************************************************\n");
	APP_DBG("sys clk =%ld\n",Clock_SysClockFreqGet());

#ifdef CFG_APP_IDLE_MODE_EN
 	IdleModeConfig();
#endif
	flash_table_init();
	sys_parameter_init();

#ifdef CFG_APP_BT_MODE_EN
	bt_em_size_init();
#endif
/*{  
	uint64_t tem;  
	uint32_t Chip_ID_High;  
	uint32_t Chip_ID_Low;  
	tem = Chip_IDGet();  
	Chip_ID_High = (tem>> 32) & 0xffffffff;  
	Chip_ID_Low= (tem) & 0xffffffff;  
	APP_DBG("Chip_ID == %llx\n",tem);  
	APP_DBG("Chip_ID_High == %x\n",Chip_ID_High);  
	APP_DBG("Chip_ID_Low == %x\n",Chip_ID_Low); 
}
*/
#if FLASH_BOOT_EN
    report_up_grate();
#endif
    ResetFlagGet(RstFlag);

	APP_DBG("Audio Decoder Version: %s\n", (unsigned char *)audio_decoder_get_lib_version());
    APP_DBG("Driver Version: %s %x\n", GetLibVersionDriver(),Chip_Version());
#ifdef CFG_FUNC_LRC_EN
    APP_DBG("Lrc Version: %s\n", GetLibVersionLrc()); 
#endif
#ifdef CFG_APP_BT_MODE_EN
    APP_DBG("BtLib Version: %s\n", (unsigned char *)GetLibVersionBt());
#endif
#ifdef CFG_RES_FLASHFS_EN
	APP_DBG("FlashFSLib Version: %s\n", (unsigned char *)GetLibVersionFlashFS());
#endif
	APP_DBG("Fatfs presearch acc Lib Version: %s\n", (unsigned char *)GetLibVersionFatfsACC());
#ifdef CFG_FUNC_ALARM_EN
	APP_DBG("RTC Version: %s\n", GetLibVersionRTC());//bkd 
#endif
	APP_DBG("\n");
#ifdef LED_IO_TOGGLE
	LedPortInit();  //�ڴ�֮�����ʹ��LedOn LedOff �۲� ����ʱ�� �ر����߷�
#endif
#ifdef CFG_FUNC_RTC_EN
#ifdef CFG_RES_RTC_EN
	RTC_IntEnable();
	NVIC_EnableIRQ(Rtc_IRQn);
#endif
	RTC_ServiceInit(RstFlag&0x01);
#endif
	RTC_SecGet();//OWEN: ����ȥ��

#if !defined(CFG_FUNC_STRING_CONVERT_EN) && !defined(USE_DBG_CODE)
	pvApplicationIdleHook = vApplicationIdleHook;
	ptrace_TASK_SWITCHED_IN = trace_TASK_SWITCHED_IN;
	ptrace_TASK_SWITCHED_OUT = trace_TASK_SWITCHED_OUT;
#endif

#ifdef CFG_APP_BT_MODE_EN
	bt_api_init();
#ifdef BT_TWS_SUPPORT
	tws_time_init();//TWS RTCУ׼
#else
	__nds32__mtsr(0,NDS32_SR_PFMC0);
	__nds32__mtsr(1,NDS32_SR_PFM_CTL);
#endif
#else
	__nds32__mtsr(0,NDS32_SR_PFMC0);
	__nds32__mtsr(1,NDS32_SR_PFM_CTL);
#endif

	uart_switch = 1;
	
	MainAppTaskStart();
	vTaskStartScheduler();

	while(1);

}

