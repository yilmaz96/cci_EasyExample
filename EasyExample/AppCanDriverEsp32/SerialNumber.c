/*
 * SerialNumber.c
 *
 *  Created on: 29.09.2020
 *      Author: hoepffr
 */

#include <stdio.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"

#define TAG              "SerialNumber"

uint32_t  u32SeriNoGet(void)
{



    uint8_t b[6];
    esp_efuse_mac_get_default(b);

    b[0] = 0x00;
    b[1] = 0x00;
    b[2] = 0x00;
    b[3] = b[3] & 0x1f; //cut 24 bits to 21


    ESP_LOGI(TAG,"SHORT-MAC = ## " MACSTR " ##", MAC2STR(b));


	return b[3] << 16 | b[4] << 8 | b[5];

}


