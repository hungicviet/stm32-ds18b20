/*********************************************
* @Projects: thaco-toliet-busy-temp
* @Author: Steven
* @Date:   2016-03-31 10:13:56
* @Last Modified by:   Steven
* @Last Modified time: 2016-04-02 00:17:19
**********************************************/
#include "ds18b20.h"


#define DS18B20_PORT GPIOB
#define DS18B20_PIN  GPIO12



#define output_low()  gpio_clear(DS18B20_PORT, DS18B20_PIN)
#define output_high() gpio_set(DS18B20_PORT, DS18B20_PIN)
#define delay_us(n)   tick_wait_us(n)

//uint8_t ds18b20_reset(void);


void ds18b20_init(void)
{
  gpio_set_mode(DS18B20_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, DS18B20_PIN);
  ds18b20_reset();
}

uint16_t ds18b20_reset(void)
{
  uint16_t result;
  delay_us(1);  //0uS
  output_low();
  delay_us(480);  //480uS
  output_high();
  delay_us(70);  //70uS
  result = gpio_get(DS18B20_PORT, DS18B20_PIN)&DS18B20_PIN;
  delay_us(410);  //410uS
  if(result == DS18B20_PIN)
    return 0;
  else
    return 1;
}

void ds18b20_write_bit(uint8_t bit)
{
  if(bit)
  {
    /*Write bit 1*/
    output_high();
    delay_us(1);//1us
    output_low();
    delay_us(5);
    output_high();
    delay_us(45);
  }
  else
  {
    /*Write bit 0*/
    output_high();
    delay_us(1);//1us
    output_low();
    delay_us(45);
    output_high();
    delay_us(10);
  }
}

uint16_t ds18b20_read_bit(void)
{
  uint16_t result;
  output_low();
  delay_us(5);
  output_high();
  delay_us(8);
  result = gpio_get(DS18B20_PORT, DS18B20_PIN)&DS18B20_PIN;
  delay_us(120);
  if(result == DS18B20_PIN)
    return 1;
  else
    return 0;
}


void ds18b20_write_byte(uint8_t data)
{
  uint8_t loop;
  for(loop=0; loop<8; loop++)
  {
    ds18b20_write_bit(data & 0x01);
    data >>= 1;
  }
}

/*void ds18b20_write_byte(uint8_t data)
{
  uint8_t loop;
  for(loop=0; loop<8; loop++)
  {
    output_low();
    delay_us(2);
    if(data&0x80 ==1)
      output_high();
    else
      output_low();
    delay_us(60);
    output_high();
    data >>= 1;

  }
}*/
uint8_t ds18b20_read_byte(void)
{
  uint8_t loop;
  uint8_t result = 0;
  for (loop = 0; loop < 8; ++loop)
  {
    result >>= 1;
    if(ds18b20_read_bit)
      result |= 0x80;

  }
  return result;
}

uint16_t ds18b20_read(void)
{
  uint8_t busy = 0;
  uint8_t temp1, temp2;
  uint16_t temp;
  ds18b20_reset();
/*  ds18b20_write_byte(0xCC);*/
  ds18b20_write_byte(0x44);
/*  while(busy==0)
    busy = ds18b20_read_byte();
  ds18b20_reset();
  ds18b20_write_byte(0xCC);*/
  ds18b20_write_byte(0xBE);
  temp1 = ds18b20_read_byte();
  temp2 = ds18b20_read_byte();
  temp = ((uint16_t)temp1 << 8) | temp2;
  delay_us(200);
  gpio_toggle(GPIOB, GPIO9);
  return temp;
}




