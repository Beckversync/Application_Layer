/*
 * Kita_MP3_PlayerKit.h
 *
 *  Created on: Oct 15, 2024
 *      Author: ASUS
 */

#ifndef INC_KITA_MP3_PLAYERKIT_H_
#define INC_KITA_MP3_PLAYERKIT_H_
#include "string.h"
#include "stm32f1xx_hal.h"

/*OFFSET PROTOTYPE*/
#define __version_offset 		1
#define __len_offset 			2
#define __command_offset 		3
#define __feedback_offset		4
#define __para1_offset			5
#define __para2_offset			6
#define __checksum_offset		7

/*COMMAND Code*/

/*NO REPLY*/
#define NEXT 					0x01
#define PREVIOUS 				0x02
#define SPECIFY_TRACKING 		0x03
#define INCREASE_VOLUMN			0x04
#define DECREASE_VOLUMN			0x05
#define SPECIFY_VOLUMN			0x06
#define SPECIFY_EQ				0x07
#define SPECIFY_PLAYBACK_MODE	0x08
#define SPECIFY_PLAYBACK_SOURCE	0x09
#define ENTER_STANDBY_MODE		0x0A
#define NORMAL_WORKING			0x0B
#define RESET_MODULE			0x0C
#define PLAYBACK				0x0D
#define PAUSE					0x0E
#define SPECIFY_PLAYBACK_FOLDER	0x0F
#define VOLUMN_ADJUST_SET		0x10
#define SET_REPEAT_PLAY_ALL		0x11
#define SET_REPEAT_PLAY_FOLDER	0x17
#define SET_REPEAT_PLAY			0x19

/*QUERY SYSTEM PARAMETERS*/
#define INITIALIZATION_PARA					0x3F
#define ERROR								0x40
#define REPLY								0x41
#define QUERY_CURRENT_STATUS				0x42
#define QUERY_CURRENT_VOLUMN				0x43
#define QUERY_CURRENT_EQ					0x44
#define QUERY_CURRENT_PLAYBACK_MODE 		0x45
#define QUERY_CURRENT_SOFTWARE_VERSION 		0x46
#define QUERY_CURRENT_TOTAL_FILES_TFCARD	0x47
#define QUERY_CURRENT_TOTAL_FILES_UDISK		0x48
#define QUERY_CURRENT_TOTAL_FILES_FLASH		0x49
#define QUERY_CURRENT_TRACK_TFCARD			0x4B
#define QUERY_CURRENT_TRACK_UDISK			0x4C
#define QUERY_CURRENT_TRACK_FLASH			0x4D

#define UDISK					0x01
#define TFCARD					0x02
#define PLAYER_FLASH			0x03
#define UDISK_FLASH				0x04

#define EQ_NORMAL				0x00
#define EQ_POP					0x01
#define EQ_ROCK					0x02
#define EQ_JAZZ					0x03
#define EQ_CLASSIC				0x04
#define EQ_BASE					0x05

#define PLAYBACK_MODE_REPEAT				0x00
#define PLAYBACK_MODE_FOLDER_REPEAT			0x01
#define PLAYBACK_MODE_SINGLE_REPEAT			0x02
#define PLAYBACK_MODE_RANDOM				0x03

#define STOP_REPEAT_PLAY_ALL	0x00
#define REPEAT_PLAY_ALL			0x01

#define REPEAT_PLAY				0x00
#define STOP_REPEAT_PLAY		0x01

/*KITA HOMEKIT DEFINE*/
#define WAITING_SONG_FOLDER		0x01
#define SOUND_EFFECT_FOLDER		0x02


extern UART_HandleTypeDef * kit_uart;

extern uint8_t replyFlag;
extern uint8_t message [10];
extern uint8_t reply [10];
extern uint8_t volumn;

void MP3Player_Init (UART_HandleTypeDef * uart, uint8_t playbackSource);

void setNormalWorking();
void setPlaybackSource (uint8_t playbackSource);
void setVolumn (uint8_t num);
void increaseVolumn ();
void decreaseVolumn ();
void setEQ (uint8_t EQ_Mode);
uint8_t playFile (uint16_t fileIdx);
uint8_t playFolderFile (uint8_t folderIdx, uint8_t fileIdx);

void loopFolder (uint8_t folderNum);
void loopAll ();
void loopFile (uint16_t fileIdx);
void loopEnable ();
void loopDisable ();

void play ();
void pause ();
void next ();
void previous ();

void reset ();
void resetMessage ();

void checkSum ();

#endif /* INC_KITA_MP3_PLAYERKIT_H_ */
