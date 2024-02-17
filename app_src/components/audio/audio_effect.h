/*******************************************************
 *         MVSilicon Audio Effects Process
 *
 *                All Right Reserved
 *******************************************************/

#ifndef __AUDIO_EFFECT_H__
#define __AUDIO_EFFECT_H__

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus

#include "audio_core_api.h"

//��ЧApply������ָ��
typedef void (*AudioEffectApplyFunc)(void *effectUint, int16_t *pcm_in, int16_t *pcm_out, uint32_t n);


#define AUDIO_EFFECT_GROUP_NUM			3//��Ч�б�����
#define AUDIO_EFFECT_NODE_NUM			12//һ����Ч������

#define AUDIO_EFFECT_SUM				(AUDIO_EFFECT_GROUP_NUM * AUDIO_EFFECT_NODE_NUM)

typedef enum __NodeType
{
	NodeType_Normal = 0,//����
	NodeType_Bifurcate,	//һ��Ϊ��
	NodeType_Group1,	//�ֲ��ĵ�һ����ʼ�ڵ�
	NodeType_Group2,	//�ֲ��ĵڶ�����ʼ�ڵ�
	NodeType_Group3,	//�ֲ��ĵ�������ʼ�ڵ�
	NodeType_Mix,		//�ֲ��ϲ��ڵ�
	NodeType_Vol,		//5
	NodeType_AEC,		//6,AECΪ�����㷨����Ҫ���⴦��
//	NodeType_TONE,		//7,TREB BASS����
//	NodeType_EqMode,	//8,EQģʽ�л�		//�Ż�EQģʽ�л�
} NodeType;

/*��Ч�ڵ�*/
typedef struct __EffectNode
{
	uint8_t					Enable;
	uint8_t					EffectType;		//��Ч����
	uint8_t					NodeType;		//�ڵ������,�Ƿ��зֲ棬�Ƿ�ϲ����Ƿ�Ϊ�ֲ�֮�����һ����ʼ�ڵ�
	uint8_t					Width;			//��Ч��λ��
	uint8_t					Index;			//λ��������Ч�е�λ��
	void*		 			EffectUnit; 	//������
	AudioEffectApplyFunc	FuncAudioEffect;//��Ч�ڵ㴦����
} EffectNode;

typedef struct __EffectNodeList
{
	uint8_t		Channel;		//���ڱ�ʾ��Ч��������Ϊ������������������һ����Ч�ڵ����Ҫ����һ��
	EffectNode	EffectNode[AUDIO_EFFECT_NODE_NUM];
} EffectNodeList;

extern EffectNodeList  gEffectNodeList[AUDIO_EFFECT_GROUP_NUM];
//��ǰģʽ��ʹ�õ���Чλ����Ч����б��еı��
extern uint8_t AudioModeIndex ;

//flash �洢��Ч����, ������Ч�ڵ�
bool AudioEffectParsePackage(uint8_t* add, uint16_t PackageLen, uint8_t* CommParam, bool IsReload);

void EffectPcmBufClear(uint32_t SampleLen);
void EffectPcmBufMalloc(uint32_t SampleLen);
void AudioEffectsInit(void);
void AudioEffectsDeInit(void);
void AudioEffectsLoadInit(bool IsReload, uint8_t mode);
void AudioEffectProcess(AudioCoreContext *pAudioCore);
void AudioMusicProcess(AudioCoreContext *pAudioCore);
void AudioBypassProcess(AudioCoreContext *pAudioCore);
void AudioEffectProcessBTHF(AudioCoreContext *pAudioCore);
void AudioNoAppProcess(AudioCoreContext *pAudioCore);
void EffectPcmBufRelease(void);

void du_efft_fadein_sw(int16_t* pcm_in, uint16_t pcm_length, uint16_t ch);		
void du_efft_fadeout_sw(int16_t* pcm_in, uint16_t pcm_length, uint16_t ch);		

uint8_t FineAudioEffectParamasIndex(uint8_t mode);
uint8_t GetAudioEffectParamasIndex(void);
extern bool IsEffectChange;
extern bool IsEffectChangeReload;

void AudioAPPDigitalGianProcess(uint32_t AppMode);
#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__AUDIO_EFFECT_H__
