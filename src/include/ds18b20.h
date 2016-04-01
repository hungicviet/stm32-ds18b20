/*********************************************
* @Projects: thaco-toliet-busy-temp
* @Author: Steven
* @Date:   2016-03-31 10:13:56
* @Last Modified by:   Steven
* @Last Modified time: 2016-03-31 10:14:01
**********************************************/

#ifndef DS18B20_H
#define DS18B20_H

#include <libopencm3/stm32/gpio.h>
#include "tick.h"
#include "stdlib.h"

void ds18b20_init(void);
uint16_t ds18b20_reset(void);
void ds18b20_write(uint8_t data);
uint16_t ds18b20_read(void);
void ds18b20_write_bit(uint8_t bit);
uint16_t ds18b20_read_bit(void);
void ds18b20_write_byte(uint8_t data);
uint8_t ds18b20_read_byte(void);
#endif /* DS18B20_H */


