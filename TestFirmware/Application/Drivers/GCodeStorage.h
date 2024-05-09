/*
 * GCodeStorage.h
 *
 *  Created on: May 9, 2024
 *      Author: loek
 */

#ifndef DRIVERS_GCODESTORAGE_H_
#define DRIVERS_GCODESTORAGE_H_

#include <fatfs.h>

namespace Drivers {

struct SystemConfiguration{

};

struct SystemStatusReport{

};

class GCodeStorage {
public:

	GCodeStorage() :
			m_initialized(false) {
	}

	bool Init();

private:
	bool MountFileSystem();
	bool InitGcodeFile();
	bool InitConfigFile();
	bool InitStatusFile();

    static constexpr size_t FATFS_WORK_BUFFER_SIZE = 2048;
	static constexpr const char *GCODE_FILE_NAME = "GCD.CBR";
	static constexpr const char *CONFIG_FILE_NAME = "CONFIG.CBR";
	static constexpr const char *STATUS_FILE_NAME = "STATUS.CBR";

	bool m_initialized;
	FATFS m_fileSystem;
	FIL m_gcodeFile;
	FIL m_configFile;
	FIL m_statusFile;
};

}

#endif /* DRIVERS_GCODESTORAGE_H_ */
