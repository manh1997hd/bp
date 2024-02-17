/*
 * mode_task_api.h
 *
 *  Created on: Mar 30, 2021
 *      Author: piwang
 */

#ifndef _MODE_TASK_API_H_
#define _MODE_TASK_API_H_
#include "type.h"


//����ϵͳ��׼ͨ·
bool ModeCommonInit(void);

//�����׼ͨ·���ã�ģʽ�˳���δ����ʱ ��׼ͨ·��̨������ģʽ�ڼ�����Ч��
void ModeCommonDeinit(void);

//��ģʽ�µ�ͨ����Ϣ����
void CommonMsgProccess(uint16_t Msg);

bool AudioIoCommonForHfp(uint32_t sampleRate, uint16_t gain, uint8_t gainBoostSel);

void tws_device_close(void);

void tws_device_open_isr(void);

void PauseAuidoCore(void);


#endif /* _MODE_TASK_API_H_ */
