const unsigned char AudioEffectAECParam_HFP[344]={

0x03,/*PGA0*/
0x00, 0x00,/*line1_left_enable*/
0x00, 0x00,/*line1_right_enable*/
0x00, 0x00,/*line2_left_enable*/
0x00, 0x00,/*line2_right_enable*/
0x00, 0x00,/*line4_left_enable*/
0x00, 0x00,/*line4_right_enable*/
0x00, 0x00,/*line5_left_enable*/
0x00, 0x00,/*line5_right_enable*/
0x3F, 0x00,/*line1_left_gain*/
0x3F, 0x00,/*line1_right_gain*/
0x3F, 0x00,/*line2_left_gain*/
0x3F, 0x00,/*line2_right_gain*/
0x3F, 0x00,/*line4_5_left_gain*/
0x3F, 0x00,/*line4_5_right_gain*/
0x3F, 0x00,/*reserved*/
0x3F, 0x00,/*reserved*/
0x00, 0x00,/*Diff_input_enable*/
0x00, 0x00,/*Diff_left_gain*/
0x00, 0x00,/*Diff_right_gain*/

0x04,/*ADC0*/
0x03, 0x00,/*enable*/
0x00, 0x00,/*mute*/
0x00, 0x10,/*left_volume*/
0x00, 0x10,/*right_volume*/
0x00, 0x00,/*LR_swap*/
0x00, 0x00,/*hpc*/
0x01, 0x00,/*dc_blocker*/

0x06,/*PGA1*/
0x00, 0x00,/*line3_left_mic1_enable*/
0x00, 0x00,/*line3_right_mic2_enable*/
0x1F, 0x00,/*line3_left_mic1_gain*/
0x00, 0x00,/*line3_left_mic1_gain_boost*/
0x1F, 0x00,/*line3_right_mic2_gain*/
0x00, 0x00,/*line3_right_mic2_gain_boost*/
0x00, 0x00,/*mic_line_sel*/

0x07,/*ADC1*/
0x03, 0x00,/*enable*/
0x00, 0x00,/*mute*/
0x00, 0x10,/*left_volume*/
0x00, 0x10,/*right_volume*/
0x00, 0x00,/*LR_swap*/
0x00, 0x00,/*hpc*/
0x01, 0x00,/*dc_blocker*/

0x08,/*AGC1*/
0x00, 0x00,/*mode*/
0x00, 0x00,/*max_level*/
0x00, 0x00,/*target_level*/
0x00, 0x00,/*max_gain*/
0x00, 0x00,/*min_gain*/
0x00, 0x00,/*gain_offset*/
0x0A, 0x00,/*frame_time*/
0x0A, 0x00,/*hold_time*/
0x0A, 0x00,/*attack_time*/
0x0A, 0x00,/*decay_time*/
0x00, 0x00,/*noise_gate_enable*/
0x05, 0x00,/*noise_gate_threshold*/
0x00, 0x00,/*noise_gate_mode*/
0x00, 0x00,/*noise_gate_hold_time*/

0x09,/*DAC0*/
0x03, 0x00,/*enable*/
0x00, 0x00,/*mute*/
0x00, 0x10,/*left_volume*/
0x00, 0x10,/*right_volume*/
0x00, 0x00,/*dither*/
0x00, 0x00,/*scramble*/
0x00, 0x00,/*mode*/

0x0A,/*DAC1*/
0x01, 0x00,/*enable*/
0x00, 0x00,/*mute*/
0x00, 0x10,/*right_volume*/
0x00, 0x00,/*dither*/
0x00, 0x00,/*scramble*/

0xFC,/*USER_TAG*/
0x05,/*length*/
0xFF,
0x01,
0x02,
0x03,
0x04,

0x81,/*1:Noise Suppressor	*/
0x01,
0x6C,0xEE,
0x03,0x00,
0x02,0x00,
0x64,0x00,

0x82,/*1:EQ        		*/
0x01,
0x00,0x00,
0x00,0x00,
0x01,0x00,
0x00,0x00,
0x64,0x00,
0xD4,0x02,
0x00,0x00,
0x01,0x00,
0x00,0x00,
0xE8,0x03,
0xD4,0x02,
0x00,0x00,
0x01,0x00,
0x00,0x00,
0x88,0x13,
0xD4,0x02,
0x00,0x00,
0x01,0x00,
0x00,0x00,
0x10,0x27,
0xD4,0x02,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,

0x83,/*1:AEC       		*/
0x01,
0x02,0x09,
0x65,0x63,
0x68,0x6F,
0x4C,0x65,
0x76,0x65,
0x6C,0x02,
0x00,0x00,
0x05,0x00,
0x04,0x00,
0x0A,0x4E,
0x6F,0x69,
0x73,0x65,
0x4C,0x65,
0x76,0x65,
0x6C,0x02,
0x00,0x00,
0x05,0x00,
0x00,0x00,

0x84,/*1:Gain      		*/
0x01,
0x00,0x00,
0xBA,0x0B,

0x85,/*1:DRC				*/
0x01,
0x2C,0x01,
0x00,0x00,
0xD4,0x02,
0xD4,0x02,
0x00,0x00,
0x00,0x00,
0xAF,0xFC,
0x64,0x00,
0x64,0x00,
0x64,0x00,
0x01,0x00,
0x01,0x00,
0x01,0x00,
0xE8,0x03,
0xE8,0x03,
0xE8,0x03,
0x00,0x10,
0x00,0x10,

};