/*
 * Application.cpp
 *
 *  Created on: May 8, 2024
 *      Author: loek
 */

#include <SEGGER_RTT.h>
#include <stdio.h>
#include <fatfs.h>
#include <cbor.h>

#include "LightTask.h"

int _read(int file, char *ptr, int len){
	return SEGGER_RTT_Read(0, ptr, len);
}

void PrintBanner() {
	SEGGER_RTT_TerminalOut(0, RTT_CTRL_CLEAR);
	SEGGER_RTT_TerminalOut(0, RTT_CTRL_TEXT_BRIGHT_BLUE);
	SEGGER_RTT_TerminalOut(0, RTT_CTRL_BG_WHITE);
	printf("+-----------------------------------------------------+\n");
	printf("| RTT terminal for GCODE processor                    |\n");
	printf("| Made by: Loek Lankhorst                             |\n");
	printf("+-----------------------------------------------------+\n");
	SEGGER_RTT_TerminalOut(0, RTT_CTRL_RESET);
}

extern "C" void StartApplication(void) {

	PrintBanner();

	if (!Tasks::Light::Start()) {
		fprintf(stderr,"Light task startup resulted in a failure!\n");
		Error_Handler();
	}

	Tasks::Light::SetSystemStatus(Tasks::Light::SystemStatus::OK);

	const char *fileName = "lex.cbr";
	FATFS fs;
	FRESULT res;

	printf("Mounting storage device...\n");
	res = f_mount(&fs, "", 1);

	if (res == FR_NO_FILESYSTEM) {
		char work_buffer[_MAX_SS];

		res = f_mkfs("", FM_ANY, 0, work_buffer, sizeof(work_buffer));
		assert(res == FR_OK);
		if (res == FR_OK) {

			printf("Created new file system\n");
			res = f_mount(&fs, "", 1);
		} else{
			fprintf(stderr, "Error from f_mkfs: %u\n", res);
		}
	}

	if (res == FR_OK) {
		uint32_t freeClust;
		FATFS *fs_ptr = &fs;
		res = f_getfree("", &freeClust, &fs_ptr); // Warning! This fills fs.n_fatent and fs.csize!
		if (res != FR_OK) {
			fprintf(stderr, "f_getfree() failed, res = %d\r\n", res);
		} else {
			printf("f_getfree() returned free clusters: %lu\n", freeClust);
			printf("Storage device is mounted successfully\n");
		}

	}

	Tasks::Light::SetXBrightness(100);
	Tasks::Light::SetXPercentage(50);
	Tasks::Light::SetSystemStatus(Tasks::Light::SystemStatus::OK);;

	if (res != FR_OK) {
		fprintf(stderr, "Failed to mount device\n");
		for (;;) {
			Error_Handler();
		}
	}
	{
		SEGGER_RTT_TerminalOut(0, RTT_CTRL_TEXT_BRIGHT_BLUE);
		SEGGER_RTT_TerminalOut(0, RTT_CTRL_BG_WHITE);
		printf(
				"---------------------------------------------- Open root dir\n");
		SEGGER_RTT_TerminalOut(0, RTT_CTRL_RESET);

		DIR dir;
		res = f_opendir(&dir, "/");
		if (res != FR_OK) {
			printf("f_opendir() failed, res = %d\n", res);
		}

		FILINFO fileInfo;
		uint32_t totalFiles = 0;
		uint32_t totalDirs = 0;
		printf("--------\r\nRoot directory:\n");
		for (;;) {
			res = f_readdir(&dir, &fileInfo);
			if ((res != FR_OK) || (fileInfo.fname[0] == '\0')) {
				break;
			}

			if (fileInfo.fattrib & AM_DIR) {
				printf("  DIR  %s\n", fileInfo.fname);
				totalDirs++;
			} else {
				printf("  FILE %s\n", fileInfo.fname);
				totalFiles++;
			}
		}
		printf("Found %lu directories and %lu files\n", totalDirs, totalFiles);
	}
	SEGGER_RTT_TerminalOut(0, RTT_CTRL_TEXT_BRIGHT_BLUE);
	SEGGER_RTT_TerminalOut(0, RTT_CTRL_BG_WHITE);
	printf("---------------------------------------------- Create new file\n");
	SEGGER_RTT_TerminalOut(0, RTT_CTRL_RESET);

	if (res == FR_OK) {
		printf("Check if file.cbr exists\n");
		FILINFO writeFileInfo;
		FRESULT stat_result = f_stat(fileName, &writeFileInfo);

		if (stat_result == FR_NO_FILE) {
			FIL cborWriteFile;
			printf("Opening file file.cbr to write, because it didn't exist\n");
			res = f_open(&cborWriteFile, fileName, FA_WRITE | FA_CREATE_ALWAYS);

			if (res == FR_OK) {

				printf("Encoding CBOR...\n");
				uint8_t buf[1024] = { 0 };
				memset((void*) buf, 0, sizeof(buf));
				snprintf((char*) buf, 1024, "Hello world!\n");
				//				CborEncoder encoder, mapEncoder;
				//				cbor_encoder_init(&encoder, buf, sizeof(buf), 0);
				//				cbor_encoder_create_map(&encoder, &mapEncoder, 1);
				//				cbor_encode_text_stringz(&mapEncoder, "foo");
				//				cbor_encode_boolean(&mapEncoder, true);
				//				cbor_encoder_close_container(&encoder, &mapEncoder);
				//
				//				size_t length = cbor_encoder_get_buffer_size(&encoder, buf);
				size_t written_bytes;

				printf("Trying to write %u bytes\n", strlen((const char*) buf));
				res = f_write(&cborWriteFile, buf, strlen((const char*) buf),
						&written_bytes);
				printf("Write result: %i, written bytes: %u\n", res,
						written_bytes);

				if (res == FR_OK) {
					res = f_sync(&cborWriteFile);
					if (res != FR_OK) {
						printf("Sync failed with return value: %u\n", res);
					}
				}

				f_close(&cborWriteFile);
			} else {
				printf("[err] Error opening file for writing\n");
			}
		} else if (res != FR_OK) {
			printf("[err] Error: %i\n", stat_result);
		}

	}

	if (res == FR_OK) {
		FILINFO fileInfo;
		FRESULT stat_result = f_stat(fileName, &fileInfo);

		if (stat_result == FR_NO_FILE) {
			printf("[err] File not found after writing...\n");
		}
	}

	SEGGER_RTT_TerminalOut(0, RTT_CTRL_TEXT_BRIGHT_BLUE);
	SEGGER_RTT_TerminalOut(0, RTT_CTRL_BG_WHITE);
	printf("---------------------------------------------- Read file\n");
	SEGGER_RTT_TerminalOut(0, RTT_CTRL_RESET);

	if (res == FR_OK) {
		FIL cborReadFile;
		printf("Opening file file.cbr to read\n");
		res = f_open(&cborReadFile, fileName, FA_READ);
		if (res == FR_OK) {
			printf("Opened file.cbr\n");
			uint8_t cbor_buffer[2048];
			size_t bytes_read;
			res = f_read(&cborReadFile, cbor_buffer, sizeof(cbor_buffer),
					&bytes_read);

			printf("Reading file result: %i, bytes read: %u\n", res,
					bytes_read);

			if (bytes_read && res == FR_OK) {
				CborParser parser;
				CborValue value;
				CborError cborError = cbor_parser_init(cbor_buffer, bytes_read,
						0, &parser, &value);
				printf("CBOR parsing result: %s\n",
						cbor_error_string(cborError));
			}

			f_close(&cborReadFile);
		} else {
			printf("Could not open file... Error code was: %u\n", res);
		}

	}
	{
		SEGGER_RTT_TerminalOut(0, RTT_CTRL_TEXT_BRIGHT_BLUE);
		SEGGER_RTT_TerminalOut(0, RTT_CTRL_BG_WHITE);
		printf(
				"---------------------------------------------- Open root dir\n");
		SEGGER_RTT_TerminalOut(0, RTT_CTRL_RESET);

		DIR dir;
		res = f_opendir(&dir, "/");
		if (res != FR_OK) {
			printf("f_opendir() failed, res = %d\n", res);
		}

		FILINFO fileInfo;
		uint32_t totalFiles = 0;
		uint32_t totalDirs = 0;
		printf("--------\r\nRoot directory:\n");
		for (;;) {
			res = f_readdir(&dir, &fileInfo);
			if ((res != FR_OK) || (fileInfo.fname[0] == '\0')) {
				break;
			}

			if (fileInfo.fattrib & AM_DIR) {
				printf("  DIR  %s\n", fileInfo.fname);
				totalDirs++;
			} else {
				printf("  FILE %s\n", fileInfo.fname);
				totalFiles++;
			}
		}
		printf("Found %lu directories and %lu files\n", totalDirs, totalFiles);
	}

	if (res == FR_OK) {
		f_unlink(fileName);
	}

	res = f_mount(NULL, "", 0);
	if (res != FR_OK) {
		printf("Unmount failed, res = %d\r\n", res);

	}

	printf("Finished task, entering infinite delay\n");
}
