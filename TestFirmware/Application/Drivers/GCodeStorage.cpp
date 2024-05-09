/*
 * GCodeStorage.cpp
 *
 *  Created on: May 9, 2024
 *      Author: loek
 */

#include "GCodeStorage.h"

#include <cerrno>
#include <assert.h>
#include <cstdio>
#include <cbor.h>

#define EXECUTE_IF_SUCCESS(value, function) do{value = (value) ? function() : value;}while(0)

namespace {



bool SetupFile(FIL *file, const char *fileName) {
	uint8_t fileMode = FA_READ | FA_WRITE;
	FRESULT result = f_stat(fileName, nullptr);

	if (result == FR_NO_FILE || result == FR_OK) {
		fileMode |= (result == FR_NO_FILE) ? FA_CREATE_ALWAYS : FA_OPEN_APPEND;
		result = f_open(file, fileName, fileMode);
	}

	if (result != FR_OK) {
		errno = EIO;
	}

	return (result == FR_OK);
}

bool CloseFile(FIL *file) {
	return f_close(file) == FR_OK;
}



}

bool Drivers::GCodeStorage::Init() {
	assert(m_initialized == false);
	bool success = true;

	EXECUTE_IF_SUCCESS(success, MountFileSystem);

	assert(success);

	EXECUTE_IF_SUCCESS(success, InitGcodeFile);
	EXECUTE_IF_SUCCESS(success, InitConfigFile);
	EXECUTE_IF_SUCCESS(success, InitStatusFile);

	assert(success);

	m_initialized = success;

	return success;
}

bool Drivers::GCodeStorage::MountFileSystem() {
	FRESULT result = f_mount(&m_fileSystem, "", 1); // Third argument is 1 and means immediately

	if (result == FR_NO_FILESYSTEM) {
		uint8_t workBuffer[FATFS_WORK_BUFFER_SIZE];
		result = f_mkfs("", FM_ANY, 0, workBuffer, sizeof(workBuffer));

		if (result == FR_OK) {
			result = f_mount(&m_fileSystem, "", 1);
			assert(result == FR_OK);
		}
	}

	if (result != FR_OK) {
		errno = EIO;
	}
	assert(result==FR_OK);
	return (result == FR_OK);
}

bool Drivers::GCodeStorage::InitGcodeFile() {
	bool setupStatus = SetupFile(&m_gcodeFile, GCODE_FILE_NAME);
	assert(setupStatus);
	if(setupStatus){
		CloseFile(&m_gcodeFile);
	}
	return setupStatus;
}

bool Drivers::GCodeStorage::InitConfigFile() {
	bool setupStatus = SetupFile(&m_configFile, CONFIG_FILE_NAME);
	assert(setupStatus);

	if (setupStatus) {
		CloseFile(&m_configFile);
	}

	return setupStatus;
}

bool Drivers::GCodeStorage::InitStatusFile() {
	bool setupStatus = SetupFile(&m_statusFile, STATUS_FILE_NAME);
	assert(setupStatus);

	if (setupStatus) {
		CloseFile(&m_statusFile);
	}
	return setupStatus;
}
