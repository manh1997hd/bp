#ifndef _SYS_PARAM_H__
#define _SYS_PARAM_H__

#include "sys_param_typedef.h"

//ע������:
//1.�������в���Ĭ���ϵ���flash�ж�ȡ,��Ҫ��\app_src\system_config\parameter.ini���޸ġ�
//2.����Ҫ�޸�SDK��������,��\app_src\system_config\parameter.ini��,ֱ���޸�bt_LocalDeviceName
//3.����ʹ�ù̶�������,���Ʋ���bt_app_func.c��LoadBtConfigurationParams�������޸Ļ�ȡ��ʽ
//4.�˴���BT_NAME��ϵͳ������������
//5.BLE�������޸���ble�㲥����������(ble_app_func.c)
#define BT_NAME						"BP10_BT"

//trim��Χ:0x07~0x1d -- �������оƬ
#define BT_TRIM_ECO0				0x16 
//trim��Χ:0x00~0x1f
#define BT_TRIM						0x14 

/* Rf Tx Power Range 
{   level  dbm
	[23] = 8,
	[22] = 6,
	[21] = 4,
	[20] = 2,
	[19] = 0,
	[18] = -2,
	[17] = -4,
	[16] = -6,
	[15] = -8,
	[14] = -10,
	[13] = -13,
	[12] = -15,
	[11] = -17,
	[10] = -19,
	[9]  = -21,
	[8]  = -23,
	[7]  = -25,
	[6]  = -27,
	[5]  = -29,
	[4]  = -31,
	[3]  = -33,
	[2]  = -35,
	[1]  = -37,
	[0]  = -39,
}
*/
//������������ʱ���书��
#define BT_TX_POWER_LEVEL			22//19 //Tony 20221109 for delay
//�����������书��
#define BT_PAGE_TX_POWER_LEVEL		16 

//bt ��������
//0 -> ��֧����������
//1 -> ���籨�ź�����
//2 -> ʹ���ֻ���������û���򲥱�������
//3 -> ǿ��ʹ�ñ�������
enum
{
	USE_NULL_RING = 0,
	USE_NUMBER_REMIND_RING = 1,
	USE_LOCAL_AND_PHONE_RING = 2,
	USE_ONLY_LOCAL_RING = 3,
};

//BT ��̨����
//0 -> BT��̨���������ֻ�
//1 -> BT��̨���������ֻ�
//2 -> �޺�̨
enum
{
	BT_BACKGROUND_FAST_POWER_ON_OFF = 0,
	BT_BACKGROUND_POWER_ON = 1,
	BT_BACKGROUND_DISABLE = 2,
};

#define BT_NAME_SIZE				sizeof(((SYS_PARAMETER *)0)->bt_LocalDeviceName)
#define BLE_NAME_SIZE				sizeof(((SYS_PARAMETER *)0)->ble_LocalDeviceName)

extern SYS_PARAMETER sys_parameter;
extern void sys_parameter_init(void);

#endif


