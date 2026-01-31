/*

  flash.c - driver code for STM32F3xx ARM processors

  Part of grblHAL

  Copyright (c) 2021 Terje Io

  This code reads/writes the whole RAM-based emulated EPROM contents from/to flash

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <string.h>

#include "main.h"
#include "hal.h"
#define FLASH_PAGE_SIZE 0x800U
#define FLASH_TARGET (uint32_t)(FLASH_BASE + ((uint32_t)FMC_SIZE * 0x400U) - (FLASH_PAGE_SIZE))


bool memcpy_from_flash (uint8_t *dest)
{
    memcpy(dest, (uint8_t *)FLASH_TARGET, hal.nvs.size);
    return true;
}

bool memcpy_to_flash (uint8_t *source)
{
    if (!memcmp(source, (uint8_t *)FLASH_TARGET, hal.nvs.size))
        return true;

    fmc_unlock();
    /* clear all pending flags */
    fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
    fmc_page_erase(FLASH_TARGET);
    fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);

    uint16_t *data = (uint16_t *)source;
    uint32_t address = FLASH_TARGET, remaining = (uint32_t)hal.nvs.size;
    fmc_state_enum status = FMC_READY;

    while(remaining && status == FMC_READY) {
        status |= fmc_halfword_program(address, *data++);
        status |= fmc_halfword_program(address + 2, *data++);
        address += 4;
        remaining -= 4;
    }
    fmc_lock();

    return (status == FMC_READY);
}
