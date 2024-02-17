const unsigned char AECParam_HFP[328]={
                       

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

0x01,/*enable*/
0x6C, 0xEE,/*threshold*/
0x03, 0x00,/*ratio*/
0x02, 0x00,/*attack*/
0x64, 0x00,/*release*/

0x82,/*1:EQ        		*/
0x01,/*enable*/
0x00, 0x00,/*pregain*/
0x00, 0x00,/*reserved*/
0x01, 0x00,/*filter1_enable*/
0x00, 0x00,/*filter1_type*/
0x64, 0x00,/*filter1_f0*/
0xD4, 0x02,/*filter1_Q*/
0x00, 0x00,/*filter1_gain*/
0x01, 0x00,/*filter2_enable*/
0x00, 0x00,/*filter2_type*/
0xE8, 0x03,/*filter2_f0*/
0xD4, 0x02,/*filter2_Q*/
0x00, 0x00,/*fi lter2_gain*/
0x01, 0x00,/*filter3_enable*/
0x00, 0x00,/*filter3_type*/
0x88, 0x13,/*filter3_f0*/
0xD4, 0x02,/*filter3_Q*/
0x00, 0x00,/*filter3_gain*/
0x01, 0x00,/*filter4_enable*/
0x00, 0x00,/*filter4_type*/
0x10, 0x27,/*filter4_f0*/
0xD4, 0x02,/*filter4_Q*/
0x00, 0x00,/*filter4_gain*/
0x00, 0x00,/*filter5_enable*/
0x00, 0x00,/*filter5_type*/
0x00, 0x00,/*filter5_f0*/
0x00, 0x00,/*filter5_Q*/
0x00, 0x00,/*filter5_gain*/
0x00, 0x00,/*filter6_enable*/
0x00, 0x00,/*filter6_type*/
0x00, 0x00,/*filter6_f0*/
0x00, 0x00,/*filter6_Q*/
0x00, 0x00,/*filter6_gain*/
0x00, 0x00,/*filter7_enable*/
0x00, 0x00,/*filter7_type*/
0x00, 0x00,/*filter7_f0*/
0x00, 0x00,/*filter7_Q*/
0x00, 0x00,/*filter7_gain*/
0x00, 0x00,/*filter8_enable*/
0x00, 0x00,/*filter8_type*/
0x00, 0x00,/*filter8_f0*/
0x00, 0x00,/*filter8_Q*/
0x00, 0x00,/*filter8_gain*/
0x00, 0x00,/*filter9_enable*/
0x00, 0x00,/*filter9_type*/
0x00, 0x00,/*filter9_f0*/
0x00, 0x00,/*filter9_Q*/
0x00, 0x00,/*filter9_gain*/
0x00, 0x00,/*filter10_enable*/
0x00, 0x00,/*filter10_type*/
0x00, 0x00,/*filter10_f0*/
0x00, 0x00,/*filter10_Q*/
0x00, 0x00,/*filter10_gain*/

0x83,/*1:AEC       		*/
0x01,/*enable*/
0x04, 0x00, /* cur value*/
0x00, 0x00, /* min value*/

0x84,/*1:Gain      		*/
0x01,/*enable*/
0x00, 0x00,/*mute*/
0x00, 0x10,/*gain*/

0x85,/*1:DRC				*/
0x01,/*enable*/
0x00, 0x00,/*mode*/
0x01, 0x00,/*type*/
0xD4, 0x02,/*ql*/
0xD4, 0x02,/*qh*/
0x2C, 0x01,/*fc1*/
0xD0, 0x07,/*fc2*/
0x00, 0x00,/*threshold1*/
0x00, 0x00,/*threshold2*/
0x00, 0x00,/*threshold3*/
0x00, 0x00,/*threshold4*/
0x64, 0x00,/*ratio1*/
0x64, 0x00,/*ratio2*/
0x64, 0x00,/*ratio3*/
0x64, 0x00,/*ratio4*/
0x64, 0x00,/*attack1*/
0x64, 0x00,/*attack2*/
0x64, 0x00,/*attack3*/
0x01, 0x00,/*attack4*/
0x01, 0x00,/*release1*/
0x01, 0x00,/*release2*/
0x01, 0x00,/*release3*/
0xE8, 0x03,/*release4*/
0x00, 0x10,/*pregain1*/
0x00, 0x10,/*pregain2*/
0x00, 0x10,/*pregain3*/
0x00, 0x10,/*pregain4*/

};