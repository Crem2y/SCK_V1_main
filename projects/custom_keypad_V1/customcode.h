// This uses the empty space in <keyboard.h>.
// unused 0x00 (1)
// mouse and repeat speed 0x01~0x17 (23)
#define M_LB 0x01 // left_mouse
#define M_RB 0x02 // right_mouse
#define M_MB 0x03 // middle_mouse
#define M_4B 0x04 // mouse_button_4 (prev)
#define M_5B 0x05 // mouse_button_5 (next)
#define M_6B 0x06 // mouse_button_6
#define M_7B 0x07 // mouse_button_7
#define M_U  0x08 // mouse_up
#define M_D  0x09 // mouse_down
#define M_L  0x0A // mouse_left
#define M_R  0x0B // mouse_right
#define M_UL 0x0C // mouse_ul
#define M_UR 0x0D // mouse_ur
#define M_DL 0x0E // mouse_dl
#define M_DR 0x0F // mouse_dr
#define MH_U 0x10 // mouse_wheel_up
#define MH_D 0x11 // mouse_wheel_down
#define M_F  0x12 // mouse_faster
#define M_S  0x13 // mouse_slower
#define MH_F 0x14 // mouse_wheel_faster
#define MH_S 0x15 // mouse_wheel_slower
#define R_F  0x16 // repeat_faster
#define R_S  0x17 // repeat_slower
// user function 0x18~0x1F (8)
#define F_01 0x18 // function_01
#define F_02 0x19 // function_02
#define F_03 0x1A // function_03
#define F_04 0x1B // function_04
#define F_05 0x1C // function_05
#define F_06 0x1D // function_06
#define F_07 0x1E // function_07
#define F_08 0x1F // function_08

// need "HID-Project.h" (consumer)
// gamepad 0x88~0xAF (40)
#define G_LU 0x88 // game_left_stick_up
#define G_LD 0x89 // game_left_stick_down
#define G_LL 0x8A // game_left_stick_left
#define G_LR 0x8B // game_left_stick_right
#define G_RU 0x8D // game_right_stick_up
#define G_RD 0x8E // game_right_stick_down
#define G_RL 0x8E // game_right_stick_left
#define G_RR 0x8F // game_right_stick_right
#define G_DU 0x90 // game_dpad_up
#define G_DD 0x91 // game_dpad_down
#define G_DL 0x92 // game_dpad_left
#define G_DR 0x93 // game_dpad_right
#define G_ZU 0x94 // game_left_Zaxis+
#define G_ZD 0x95 // game_left_Zaxis-
#define G_ZL 0x96 // game_right_Zaxis+
#define G_ZR 0x97 // game_right_Zaxis-
#define G_01 0x98 // game_button_01
#define G_02 0x99 // game_button_02
#define G_03 0x9A // game_button_03
#define G_04 0x9B // game_button_04
#define G_05 0x9C // game_button_05
#define G_06 0x9D // game_button_06
#define G_07 0x9E // game_button_07
#define G_08 0x9F // game_button_08
#define G_09 0xA0 // game_button_09
#define G_10 0xA1 // game_button_10
#define G_11 0xA2 // game_button_11
#define G_12 0xA3 // game_button_12
#define G_13 0xA4 // game_button_13
#define G_14 0xA5 // game_button_14
#define G_15 0xA6 // game_button_15
#define G_16 0xA7 // game_button_16
#define G_17 0xA8 // game_button_17
#define G_18 0xA9 // game_button_18
#define G_19 0xAA // game_button_19
#define G_20 0xAB // game_button_20
#define G_21 0xAC // game_button_21
#define G_22 0xAD // game_button_22
#define G_23 0xAE // game_button_23
#define G_24 0xAF // game_button_24

// consumer key 0xB4~0xC0 (13)
#define C_VM 0xB4 // con_volume_mute
#define C_VU 0xB5 // con_volume_up
#define C_VD 0xB6 // con_volume_down
#define C_MP 0xB7 // con_media_play_pause
#define C_MS 0xB8 // con_media_stop
#define C_MN 0xB9 // con_media_next
#define C_MR 0xBA // con_media_prev
#define C_CA 0xBB // con_calculator
#define C_EM 0xBC // con_email
#define C_BH 0xBD // con_browser_home
#define C_BB 0xBE // con_browser_back
#define C_BF 0xBF // con_browser_forward
#define C_FI 0xC0 // con_find (not working well)

// surface dial 0xEC~0xEE (3)
#define S_B  0xEC // surface_button
#define S_CW 0xED // surface_clockwise
#define S_CC 0xEE // surface_counter_clockwise
// LED chenge 0xEF (1)
#define L_CN 0xEF // LED_change

// special function 0xFC~0xFF (4)
#define F_S1 0xFC // special_function_01
#define F_S2 0xFD // special_function_02
#define F_S3 0xFE // special_function_03
#define F_S4 0xFF // special_function_04
