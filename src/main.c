/*
 * This file is part of the libopencm3 project.
 *
 * (C) Copyright 2015 Tuan PM.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * Contributors:
 *     Tuan PM <tuanpm@live.com>
 * File: main.c
 * Created: 8:15:47 AM Nov 17, 2015 GMT+7
 */

#include "config.h"
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <libopencm3/stm32/rcc.h>

#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>
#include "cdcacm.h"
#include "tick.h"
#include "debug.h"


#include "ds18b20.h"

uint16_t n = 0;
static uint8_t data[16];


static void clock_init(void);
static uint8_t  *read_unique_id(void);

int main(void)
{


  clock_init();
  tick_init();
  cdcacm_init();
  ds18b20_init();

  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO9);
  gpio_set(GPIOC, GPIO13);

  while (1)
  {

    //ds18b20_reset();
    //ds18b20_write_bit(0);
    //tick_wait_ms(500);


    tick_wait_ms(1000);
    {
      gpio_toggle(GPIOC, GPIO13);
      sprintf(data, "%d \n", ds18b20_read());
      //ds18b20_write_byte(0xCC);
      cdcacm_send(data);

    }

    cdcacm_manage();
    /*if(tick_expire_us(&m_tick, 1000)){
      // cdcacm_send("Hello\n", sizeof("Hello\n"));
      cdcacm_send("Hello World!\n");
      gpio_toggle(GPIOC, GPIO13);
    }
*/
  }

}

static void clock_init(void)
{
  rcc_clock_setup_in_hse_8mhz_out_72mhz();
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_GPIOC);
  rcc_periph_clock_enable(RCC_GPIOD);
  rcc_periph_clock_enable(RCC_USART2);
  rcc_periph_clock_enable(RCC_AFIO);
}

static uint8_t  *read_unique_id(void)
{
  static uint8_t id_byte[12];
  static uint8_t id_hex[25];
  for(uint8_t i = 0; i < 12; i++)
  {
    id_byte[i] = *(unsigned char*)(0x1FFFF7E8 + i);
    sprintf(id_hex+i*2, "%02X", id_byte[i]);
  }
  id_hex[24]=0x0A; //new line
  return id_hex;
}

