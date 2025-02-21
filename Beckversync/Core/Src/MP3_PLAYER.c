/*
 * Kita_MP3_PlayerKit.c
 *
 *  Created on: Oct 15, 2024
 *      Author: ASUS
 */
#include "MP3_Player.h"

uint8_t replyFlag = 0;
uint8_t message [] = {0x7E, 0xFF, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEF};
uint8_t reply[sizeof(message)/sizeof(message[0])];
uint8_t volumn = 0;

UART_HandleTypeDef * kit_uart;

void MP3Player_Init (UART_HandleTypeDef * uart, uint8_t playbackSource) {
	kit_uart = uart;

	if (playbackSource < UDISK || playbackSource > UDISK_FLASH)
		playbackSource = TFCARD;

	reset();
//	para_Init(playbackSource);
	setVolumn(15);
	setEQ(EQ_NORMAL);
	setPlaybackSource(playbackSource);
//	setNormalWorking();
}

/*Reset the module*/
void reset () {
	message[__command_offset] = RESET_MODULE;
	checkSum();
	HAL_UART_Transmit(kit_uart, message, sizeof(message), 200);
	replyFlag = 0;
	HAL_Delay(3000);
	resetMessage();
}

void setNormalWorking () {
	message[__command_offset] = NORMAL_WORKING;//Normal working
	checkSum();
	HAL_UART_Transmit(kit_uart, message, sizeof(message), 200);
	HAL_Delay(200);
	resetMessage();
}

void setPlaybackSource (uint8_t playbackSource) {
	if (playbackSource < UDISK || playbackSource > UDISK_FLASH)
		playbackSource = TFCARD;

	message[__command_offset] = SPECIFY_PLAYBACK_SOURCE;
	message[__para2_offset] = playbackSource;
	checkSum();
	HAL_UART_Transmit(kit_uart, message, sizeof(message), 200);
	HAL_Delay(200);
	resetMessage();
}

void setVolumn (uint8_t num) {
	if (num < 0 || num > 30)
		return;
	volumn = num;
    message[__command_offset] = SPECIFY_VOLUMN;
    message[__para2_offset] = num;
    checkSum();
    HAL_UART_Transmit(kit_uart, message, sizeof(message), 200);
    HAL_Delay(200);
    resetMessage();
}

void increaseVolumn () {
	if (++volumn > 30)
		volumn = 30;
	message[__command_offset] = INCREASE_VOLUMN;
	checkSum();
	HAL_UART_Transmit(kit_uart, message, sizeof(message), 200);
	HAL_Delay(200);
	resetMessage();
}

void decreaseVolumn (){
	if (--volumn < 0)
		volumn = 0;
	message[__command_offset] = DECREASE_VOLUMN;
	checkSum();
	HAL_UART_Transmit(kit_uart, message, sizeof(message), 200);
	HAL_Delay(200);
	resetMessage();
}

void setEQ (uint8_t EQ_Mode) {
	if (EQ_Mode < EQ_NORMAL || EQ_Mode > EQ_BASE)
		EQ_Mode = EQ_NORMAL;
	message[__command_offset] = SPECIFY_EQ;//Set EQ to Base
	message[__para2_offset] = EQ_Mode;
	checkSum();
	HAL_UART_Transmit(kit_uart, message, sizeof(message), 200);
	HAL_Delay(200);
	resetMessage();
}

uint8_t playFile (uint16_t fileIdx) {
	if (fileIdx < 0 || fileIdx > 2999)
		return 0;

	message[__command_offset] = SPECIFY_TRACKING;
	message[__para1_offset] = (fileIdx >> 8);
	message[__para2_offset] = (fileIdx & 0xFF);
	checkSum();
	HAL_UART_Transmit(kit_uart, message, sizeof(message), 200);
	HAL_Delay(200);
	resetMessage();
	return 1;
}

uint8_t playFolderFile (uint8_t folderIdx, uint8_t fileIdx ) {
	if (folderIdx < 1 || folderIdx > 10 || fileIdx < 0 || fileIdx > 2999)
		return 0;

	message[__command_offset] = SPECIFY_PLAYBACK_FOLDER;
	message[__para1_offset] = folderIdx;
	message[__para2_offset] = fileIdx;
	checkSum();
	HAL_UART_Transmit(kit_uart, message, sizeof(message), 200);
	HAL_Delay(200);
	resetMessage();
	return 1;
}

void loopFolder (uint8_t folderNum) {
	message[__command_offset] = SPECIFY_PLAYBACK_MODE;
	message[__para2_offset] = PLAYBACK_MODE_FOLDER_REPEAT;
	checkSum();
	HAL_UART_Transmit(kit_uart, message, sizeof(message), 200);
	HAL_Delay(200);
	resetMessage();

	message[__command_offset] = SET_REPEAT_PLAY_FOLDER;
	message[__para2_offset] = folderNum;
	checkSum();
	HAL_UART_Transmit(kit_uart, message, sizeof(message), 200);
	HAL_Delay(200);
	resetMessage();
}

void loopAll () {
	message[__command_offset] = SET_REPEAT_PLAY_ALL;
	message[__para2_offset] = REPEAT_PLAY_ALL;
	checkSum();
	HAL_UART_Transmit(kit_uart, message, sizeof(message), 200);
	HAL_Delay(200);
	resetMessage();
}

void loopFile (uint16_t fileIdx) {
	message[__command_offset] = SPECIFY_PLAYBACK_MODE;
	message[__para2_offset] = fileIdx;
	checkSum();
	HAL_UART_Transmit(kit_uart, message, sizeof(message), 200);
	HAL_Delay(200);
	resetMessage();
}

void loopEnable () {
	message[__command_offset] = SET_REPEAT_PLAY;
	message[__para2_offset] = REPEAT_PLAY;
	checkSum();
	HAL_UART_Transmit(kit_uart, message, sizeof(message), 200);
	HAL_Delay(200);
	resetMessage();
}

void loopDisable () {
	message[__command_offset] = SET_REPEAT_PLAY;
	message[__para2_offset] = STOP_REPEAT_PLAY;
	checkSum();
	HAL_UART_Transmit(kit_uart, message, sizeof(message), 200);
	HAL_Delay(200);
	resetMessage();
}

void play (){
    message[__command_offset] = PLAYBACK;
    checkSum();
    HAL_UART_Transmit(kit_uart, message, sizeof(message), 200);
    HAL_Delay(200);
    resetMessage();
}

void pause () {
    message[__command_offset] = PAUSE;
    checkSum();
    HAL_UART_Transmit(kit_uart, message, sizeof(message), 200);
    HAL_Delay(200);
    resetMessage();
}

void next () {
	message[__command_offset] = NEXT;
	checkSum();
	HAL_UART_Transmit(kit_uart, message, sizeof(message), 200);
	HAL_Delay(200);
	resetMessage();
}

void previous () {
	message[__command_offset] = PREVIOUS;
	checkSum();
	HAL_UART_Transmit(kit_uart, message, sizeof(message), 200);
	HAL_Delay(200);
	resetMessage();
}

void resetMessage () {
	message[__command_offset] = 0x00;
	message[__para1_offset] = 0x00;
	message[__para2_offset] = 0x00;
	message[__checksum_offset] = 0x00;
	message[__checksum_offset] = 0x00;

//	memset(reply, 0, sizeof(reply)/sizeof(reply[0]));
}

void checkSum () {
	uint16_t sum = 0;
	for (int idx = __version_offset; idx < __checksum_offset; idx++) {
		sum += message[idx];
	}
	sum = ~sum + 1;
	message[__checksum_offset] = (sum >> 8);
	message[__checksum_offset + 1] = (sum & 0xFF);
}
