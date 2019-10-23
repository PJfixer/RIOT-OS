
#include "lcd_grove.h"


void print_test(void)
{

		printf("test !! \n");
	
}

unsigned char _displayfunction = 0x00;
unsigned char _displaycontrol = 0x00;

int lcd_sendcmd(i2c_t bus,unsigned char cmd)
{
	if(i2c_acquire(bus) != 0) // je  chope le bus
	{
		printf("lcd_sendcmd : acquire i2c fail \n");
		i2c_release(bus);  // si ca foire
		return -1;
	}
	if(i2c_write_reg(bus,LCD_ADDR,CMD_REG,cmd,0) != 0) // jecris ma commande
	{
		printf("lcd_sendcmd : Write fail \n");
		i2c_release(bus);
		return -1;
	}
	i2c_release (bus);
	printf("lcd_sendcmd : succes \n");
	return 0;
}

int lcd_sendByte(i2c_t bus,unsigned char data)
{
	if(i2c_acquire(bus) != 0) // je  chope le bus
	{
		i2c_release(bus);  // si ca foire
		return -1;
	}
	if(i2c_write_reg(bus,LCD_ADDR,DATA_REG,data,0) != 0) // jecris ma data
	{
		i2c_release(bus);
		return -1;
	}
	i2c_release(bus);
	
	return 0;
	
}

int lcd_init(i2c_t bus){
int Res;
i2c_init(bus);// Init partie LCD //Initialize displayfunction parameter for setting up LCD display
_displayfunction |= LCD_2LINE;
_displayfunction |= LCD_5x10DOTS;
xtimer_usleep(30*US_PER_MS); // Wait > 30ms
Res = lcd_sendcmd(bus,(LCD_FUNCTIONSET | _displayfunction));
if (Res != 0) 
	return  -1;
xtimer_usleep(40);// Wait > 39μs
Res = lcd_displayOn(bus);
if (Res != 0) 
	return  -1;
Res = lcd_clear(bus);
if (Res != 0) return  -1;// Init RGB lib_lcd_backlight device
Res = i2c_acquire(bus);
if (Res != 0) 
	return -1;
Res = i2c_write_reg(bus,BCK_ADDR,0,0,0);
if (Res != 0) 
	return -1;
Res = i2c_write_reg(bus,BCK_ADDR,1,0,0);
if (Res != 0) 
	return -1;
Res = i2c_write_reg(bus,BCK_ADDR,0x08,0xAA,0);
if (Res != 0) 
	return -1;
i2c_release(bus);
if (Res != 0) 
	return -1;

return 0;
}


int lcd_displayOn(i2c_t bus){
	_displaycontrol |= LCD_DISPLAYON;
	return lcd_sendcmd(bus,(LCD_DISPLAYCONTROL | _displaycontrol));
	}
	
	
int	lcd_clear(i2c_t bus){
	return lcd_sendcmd(bus,LCD_CLEARDISPLAY);
	}
	
	
	
	
void setRGB(i2c_t bus,unsigned char r, unsigned char g, unsigned char b)
{
	i2c_acquire(bus);
	i2c_write_reg(bus,BCK_ADDR,RED_REG,r,0);
	i2c_write_reg(bus,BCK_ADDR,GREEN_REG,g,0);
	i2c_write_reg(bus,BCK_ADDR,BLUE_REG,b,0);
	i2c_release(bus);
   
}

void lcd_SendStr(i2c_t bus,const char * s)
{
    while(*s)
    {
            lcd_sendByte(bus,*s++);
	}
}
