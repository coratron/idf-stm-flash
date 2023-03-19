#include "stm_option_bytes.h"

int setOptionBytes(const uint32_t address, const uint8_t size)
{
    char addressBytes[4] = {};
    char optionBytes[8];
    int ret = 0;

    addressBytes[0] = (address >> 24) & 0xFF;
    addressBytes[1] = (address >> 16) & 0xFF;
    addressBytes[2] = (address >> 8) & 0xFF;
    addressBytes[3] = (address >> 0) & 0xFF;

    readBlocks(addressBytes, optionBytes, size);

    flashLogInfo(FLASH_READ, "Checking Option Bytes\r\n");
    for (int i = 0; i < 8; i++)
        flashLogInfo(FLASH_READ, "optionBytes[%d] is: %x", i, optionBytes[i]);

    // check the status of nBOOT_SEL bit (it needs to be 0 to use the BOOT0 pin) and its complement
    if (((optionBytes[3] & 0b00000001) == 0b00000001) && ((optionBytes[7] & 0b00000001) == 0b00000000))
    {
        // change nBOOT_SEL to 0
        optionBytes[3] &= ~0b00000001;

        // change its complement to 1
        optionBytes[7] |= 0b00000001;

        flashLogInfo(FLASH_WRITE, "Updating nBOOT_SEL bit\r\n");
        ret = flashBlocks(addressBytes, (const char *)optionBytes, size);

        // reboot to apply changes
        resetSTM();

        vTaskDelay(200 / portTICK_PERIOD_MS);

        // check if update was succesful
        //  get all option bytes and their complement
        ret = !readBlocks(addressBytes, optionBytes, size);
        flashLogInfo(FLASH_READ, "Checking Option Bytes\r\n");
        for (int i = 0; i < 8; i++)
        flashLogInfo(FLASH_READ, "optionBytes[%d] is: %x", i, optionBytes[i]);
    }



    // if it is not, then change it
    if (((optionBytes[3] & 0b00000001) == 0b00000000) && ((optionBytes[7] & 0b00000001) == 0b00000001))
    {
        flashLogInfo(FLASH_READ, "Success: nBOOT_SEL bit is cleared!\r\n");
    }
    else
        flashLogInfo(FLASH_READ, "Failure clearing nBOOT_SEL bit!\r\n");

    return ret;
}