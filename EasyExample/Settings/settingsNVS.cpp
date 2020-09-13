/* ************************************************************************ */
/*!
   \file
   \brief       Helper functions for reading and writing settings to a file.

*/
/* ************************************************************************ */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include "settings.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "nvs_handle.hpp"

#if 1
#define vswprintf_s swprintf
#define vsprintf_s snprintf
#define _strtoui64 strtoull
#define vswprintf_s swprintf
#define vsprintf_s snprintf
#define sprintf_s snprintf
#define strcpy_s strcpy
#endif // 1

/* ************************************************************************ */

static const char FILENAME[] = ".\\settings.ini";
#define MAXSTRLEN      1024U
static class Settings 
{
private:
	std::shared_ptr<nvs::NVSHandle> my_handle;
public :
    Settings()
    {
    	my_handle = 0;
    }
    void init(void)
    {
		// Initialize NVS
		esp_err_t err = nvs_flash_init();
		if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) 
		{
			// NVS partition was truncated and needs to be erased
			// Retry nvs_flash_init
			ESP_ERROR_CHECK (nvs_flash_erase());
			err = nvs_flash_init();
		}
		ESP_ERROR_CHECK(err);
		// Open
		printf("\n");
		printf("Opening Non-Volatile Storage (NVS) handle... ");
		esp_err_t result;
		my_handle = nvs::open_nvs_handle("storage", NVS_READWRITE, &result);
		if (result != ESP_OK) {
			printf("Error (%s) opening NVS handle!\n", esp_err_to_name(result));
		} else {
			printf("Done\n");
		}
        return;
    }

    ~Settings()
    {
        // Close
    }


    int8_t getS8(const char section[], const char key[], const int8_t defaultValue)
    {
    	int8_t value;
    	my_handle->get_item(key, value);
    	return value;
    }

    int16_t getS16(const char section[], const char key[], const int16_t defaultValue)
    {
    	int16_t value;
    	my_handle->get_item(key, value);
    	return value;
    }

    int32_t getS32(const char section[], const char key[], const int32_t defaultValue)
    {
    	int32_t value;
    	my_handle->get_item(key, value);
    	return value;
    }

    int64_t getS64(const char section[], const char key[], const int64_t defaultValue)
    {
    	int64_t value;
    	esp_err_t error = my_handle->get_item(key, value);
        if (error != ESP_OK)
        {
        	my_handle->set_item(key, defaultValue);
            return defaultValue;
        }
    	return value;
    }

    uint8_t getU8(const char section[], const char key[], const uint8_t defaultValue)
    {
    	uint8_t value;
    	my_handle->get_item(key, value);
    	return value;
    }

    uint16_t getU16(const char section[], const char key[], const uint16_t defaultValue)
    {
    	uint16_t value;
    	my_handle->get_item(key, value);
    	return value;
    }

    uint32_t getU32(const char section[], const char key[], const uint32_t defaultValue)
    {
    	uint32_t value;
    	my_handle->get_item(key, value);
    	return value;
    }

    uint64_t getU64(const char section[], const char key[], const uint64_t defaultValue)
    {
    	uint64_t value;
    	my_handle->get_item(key, value);
    	return value;
    }

    uint64_t getX64(const char section[], const char key[], const uint64_t defaultValue)
    {
    	uint64_t value;
    	my_handle->get_item(key, value);
    	return value;
    }

    uint32_t getString(const char section[], const char key[], const char* defaultValue, char* captionOut, uint32_t captionSize)
    {
        char *captionTemp = nullptr;
        my_handle->get_string(key, captionTemp, captionSize);
        if (captionTemp == nullptr)
        {
            return 0U;
        }

        std::string caption(captionTemp);
        strcpy(captionOut, caption.c_str());
        return static_cast<uint32_t>(caption.size());
    }


    void setS8(const char section[], const char key[], const int8_t value)
    {
    	my_handle->set_item(key, value);
    }

    void setS16(const char section[], const char key[], const int16_t value)
    {
    	my_handle->set_item(key, value);
    }

    void setS32(const char section[], const char key[], const int32_t value)
    {
    	my_handle->set_item(key, value);
    }

    void setS64(const char section[], const char key[], const int64_t value)
    {
    	my_handle->set_item(key, value);
    }

    void setU8(const char section[], const char key[], const uint8_t value)
    {
    	my_handle->set_item(key, value);
    }

    void setU16(const char section[], const char key[], const uint16_t value)
    {
    	my_handle->set_item(key, value);
    }

    void setU32(const char section[], const char key[], const uint32_t value)
    {
    	my_handle->set_item(key, value);
    }

    void setU64(const char section[], const char key[], const uint64_t value)
    {
    	my_handle->set_item(key, value);
    }

    void setX64(const char section[], const char key[], const uint64_t value)
    {
    	my_handle->set_item(key, value);
    }

    void setString(const char section[], const char key[], const char value[])
    {
    	my_handle->set_string(key, value);
    }


} s_settings;



void Settings_init(void)
{
	s_settings.init();
}

uint32_t GetPrivateProfileStringA(
    const char* section,
    const char* lpKeyName,
    const char* lpDefault,
    char* lpReturnedString,
    uint32_t nSize,
    const char* lpFileName )
{
    (void)lpFileName;
    return s_settings.getString(section, lpKeyName, lpDefault, lpReturnedString, nSize);
}

bool WritePrivateProfileStringA(
    const char* section,
    const char* lpKeyName,
    const char* lpString,
    const char* lpFileName
    )
{
    s_settings.setString(section, lpKeyName, lpString);
    (void)lpFileName;
    return true;
}

uint32_t GetPrivateProfileSectionA(
    const char* section,
    char* lpReturnedString,
    uint32_t nSize,
    const char* lpFileName
    )
{
    return 0;
}

/* ************************************************************************ */

int8_t getS8(const char section[], const char key[], const int8_t defaultValue)
{
	return s_settings.getS8(section, key, defaultValue);
}

int16_t getS16(const char section[], const char key[], const int16_t defaultValue)
{
	return s_settings.getS16(section, key, defaultValue);
}

int32_t getS32(const char section[], const char key[], const int32_t defaultValue)
{
	return s_settings.getS32(section, key, defaultValue);
}

int64_t getS64(const char section[], const char key[], const int64_t defaultValue)
{
    return s_settings.getS64(section, key, defaultValue);
}

uint8_t getU8(const char section[], const char key[], const uint8_t defaultValue)
{
	return s_settings.getU8(section, key, defaultValue);
}

uint16_t getU16(const char section[], const char key[], const uint16_t defaultValue)
{
	return s_settings.getU16(section, key, defaultValue);
}

uint32_t getU32(const char section[], const char key[], const uint32_t defaultValue)
{
	return s_settings.getU32(section, key, defaultValue);
}

uint64_t getU64(const char section[], const char key[], const uint64_t defaultValue)
{
	return s_settings.getU64(section, key, defaultValue);
}

uint64_t getX64(const char section[], const char key[], const uint64_t defaultValue)
{
	return s_settings.getX64(section, key, defaultValue);
}

void getString(const char section[], const char key[], const char defaultValue[], char caption[], size_t size)
{
   static char buffer[MAXSTRLEN];
   uint32_t charCount = GetPrivateProfileStringA(section, key, NULL, buffer, sizeof(buffer), FILENAME);
   if (charCount == 0U)
   {
      WritePrivateProfileStringA(section, key, defaultValue, FILENAME);
      strcpy_s(caption, defaultValue);
   }
   else
   {
      strcpy_s(caption, buffer);
   }
}

void setS8(const char section[], const char key[], const int8_t value)
{
	return s_settings.setS8(section, key, value);
}

void setS16(const char section[], const char key[], const int16_t value)
{
	return s_settings.setS16(section, key, value);
}

void setS32(const char section[], const char key[], const int32_t value)
{
	return s_settings.setS32(section, key, value);
}

void setS64(const char section[], const char key[], const int64_t value)
{
	return s_settings.setS64(section, key, value);
}

void setU8(const char section[], const char key[], const uint8_t value)
{
	s_settings.setU8(section, key, value);
}

void setU16(const char section[], const char key[], const uint16_t value)
{
	s_settings.setU16(section, key, value);
}

void setU32(const char section[], const char key[], const uint32_t value)
{
	s_settings.setU32(section, key, value);
}

void setU64(const char section[], const char key[], const uint64_t value)
{
	s_settings.setU64(section, key, value);
}

void setX64(const char section[], const char key[], const uint64_t value)
{
	s_settings.setX64(section, key, value);
}

void setString(const char section[], const char key[], const char value[])
{
	s_settings.setString(section, key, value);
}

size_t getSection(const char section[], char string[], size_t stringSize)
{
   size_t sectionChars = 0;
   return sectionChars;
}

void clearSection(const char section[])
{
   // erase complete section
}

/* ************************************************************************ */
