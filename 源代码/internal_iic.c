#include "config.h"
#define INTERNAL_IIC_DELAY() Delay5us()

void Delay5us()		//@11.0592MHz
{
	unsigned char i;
	_nop_();
	i = 64;
	while (--i);
}

void int_iic_start()        /* --------- Initiate int_iic_start condition ---------- */   
{   
  INT_SDA = 1; INTERNAL_IIC_DELAY();INTERNAL_IIC_DELAY();
	INT_SCL = 1; INTERNAL_IIC_DELAY();INTERNAL_IIC_DELAY();  
  INT_SDA = 0; INTERNAL_IIC_DELAY();INTERNAL_IIC_DELAY();   
}   

void int_iic_stop()     /* ---------- Initiate int_iic_stop condition ----------- */   
{   
	INT_SDA = 0;   INTERNAL_IIC_DELAY();INTERNAL_IIC_DELAY();
  INT_SCL = 1;INTERNAL_IIC_DELAY();INTERNAL_IIC_DELAY();
  INT_SDA = 1;  INTERNAL_IIC_DELAY();INTERNAL_IIC_DELAY(); 
}   
   

uint8_t int_iic_write(uint8_t d)     /* ----------------------------- */   
{   
    uint8_t i;   
   	bit ack;
    INT_SCL = 0; INTERNAL_IIC_DELAY();  
    for (i = 0;i < 8; i++)   
    {   INTERNAL_IIC_DELAY();
        if (d & 0x80)   
            INT_SDA = 1; /* Send the msbits first */   
        else   
            INT_SDA = 0;   
        INTERNAL_IIC_DELAY();
				INT_SCL = 0;   INTERNAL_IIC_DELAY();
        INT_SCL = 1;   INTERNAL_IIC_DELAY();
        d = d << 1;  INTERNAL_IIC_DELAY(); /* do shift here to increase INT_SCL high time */   
        INT_SCL = 0;   INTERNAL_IIC_DELAY();
    }   
	INT_SDA = 1;
	INT_SCL = 1;
	i = 255;
	while(INT_SDA && --i)
	{
		;
	}
	ack = INT_SDA;	//接收设备的ack	_n
	INT_SCL = 0;
	INT_SDA = 1;
  return(ack);   
}   
   
   
uint8_t int_iic_read(uint8_t ack)  /* ----------------------------------- */   
{   
    uint8_t i, d;   
   
    d = 0;   
    INT_SDA = 1;INTERNAL_IIC_DELAY();            /* Let go of INT_SDA line */   
    INT_SCL = 0;INTERNAL_IIC_DELAY(); 
    for (i = 0; i < 8; i++)  /* read the msb first */   
    {   
        INT_SCL = 1;INTERNAL_IIC_DELAY();   
        d = d << 1;  INTERNAL_IIC_DELAY(); 
        d = d | (unsigned char)INT_SDA; INTERNAL_IIC_DELAY();  
        INT_SCL = 0;   INTERNAL_IIC_DELAY();
    }   
		INTERNAL_IIC_DELAY();INTERNAL_IIC_DELAY();INTERNAL_IIC_DELAY();INTERNAL_IIC_DELAY();
    INT_SDA = ack;INTERNAL_IIC_DELAY();          /* low for ack, high for nack */   
    INT_SCL = 1; INTERNAL_IIC_DELAY();  
    INT_SCL = 0; INTERNAL_IIC_DELAY();  
   
    INT_SDA = 1; INTERNAL_IIC_DELAY();         /* Release the INT_SDA line */   
    return d;   
}   

uint8_t int_iic_read_addr(uint8_t devaddr, uint8_t addr)
{
	uint8_t result = 0;
	int_iic_start();
	int_iic_write(devaddr);
	int_iic_write(addr);
	
	int_iic_start();
	int_iic_write(devaddr|0x01);
	result = int_iic_read(INTERNAL_IIC_NACK);
	int_iic_stop();
	return result;
}

void int_iic_write_addr(uint8_t devaddr, uint8_t addr, uint8_t dat)
{
	int_iic_start();
	int_iic_write(devaddr);
	int_iic_write(addr);
	int_iic_write(dat);
	int_iic_stop();
}

uint8_t int_iic_check_device(uint8_t addr)
{
	uint8_t result = 0;
	int_iic_start();
	result = int_iic_write(addr);
	int_iic_stop();
	return result;
}

uint8_t command_iic_check(COMMAND_ARGS)
{
	uint8_t addr;
	uint8_t tmpstr[12];
	keyClear();
	terminal_add_bottom("Checking IIC...");
	for(addr = 0; addr < 0x7f; ++addr)
	{
		if(int_iic_check_device(addr << 1) == 0)
		{
			sprintf(tmpstr, "Found:0x%2x", addr);
			tmpstr[11] = 0;
			terminal_add_bottom(tmpstr);
			while(keyUp.pressed == 0);
			keyUp.pressed = 0;
		}
	}
	return COMMAND_OK;
}
uint8_t command_iic_read(COMMAND_ARGS)
{
	uint8_t tmpstr[7];
	uint8_t devaddr;
	uint8_t addr;
	uint8_t result;
	
	if(argc != 2)return COMMAND_ARG_ERROR;

	devaddr = gethex8(argv[1]);	
	addr = gethex8(argv[2]);	
	
	if(int_iic_check_device(devaddr << 1))
	{
		terminal_add_bottom("No Such Device");
		return COMMAND_ERROR;
	}
	result = int_iic_read_addr(devaddr << 1, addr);	
	
	sprintf(tmpstr, "0x%2x", result);
	tmpstr[4] = 0;
	terminal_add_bottom(tmpstr);
	return COMMAND_OK;
}
uint8_t command_iic_write(COMMAND_ARGS)
{
	uint8_t devaddr;
	uint8_t addr;
	uint8_t dat;
	if(argc != 3)return COMMAND_ARG_ERROR;
	devaddr = gethex8(argv[1]);
	addr = gethex8(argv[2]);
	dat = gethex8(argv[3]);
	if(int_iic_check_device(devaddr << 1))
	{
		terminal_add_bottom("No Such Device");
		return COMMAND_ERROR;
	}
	int_iic_write_addr(devaddr << 1, addr, dat);
	terminal_add_bottom("OK");
	return COMMAND_OK;
}

uint8_t command_iic_read16(COMMAND_ARGS)
{
	uint8_t tmpstr[7];
	uint8_t devaddr;
	uint16_t addr;
	uint8_t result;
	
	if(argc != 3)return COMMAND_ARG_ERROR;

	devaddr = gethex8(argv[1]);	
	addr = gethex8(argv[2]);	
	addr <<= 8;
	addr += gethex8(argv[3]);
	devaddr <<= 1;
	if(int_iic_check_device(devaddr))
	{
		terminal_add_bottom("No Such Device");
		return COMMAND_ERROR;
	}
	int_iic_start();
	int_iic_write(devaddr);
	int_iic_write(addr >> 8);
	int_iic_write(addr);
	
	int_iic_start();
	int_iic_write(devaddr|0x01);
	result = int_iic_read(INTERNAL_IIC_NACK);
	int_iic_stop();
	
	sprintf(tmpstr, "0x%2x", result);
	tmpstr[4] = 0;
	terminal_add_bottom(tmpstr);
	return COMMAND_OK;
}

uint8_t command_iic_write16(COMMAND_ARGS)
{
	uint8_t devaddr;
	uint16_t addr;
	uint8_t dat;
	if(argc != 4)return COMMAND_ARG_ERROR;
	devaddr = gethex8(argv[1]);
	addr = gethex8(argv[2]);
	addr <<= 8;
	addr += gethex8(argv[3]);
	dat = gethex8(argv[4]);
	if(int_iic_check_device(devaddr << 1))
	{
		terminal_add_bottom("No Such Device");
		return COMMAND_ERROR;
	}
	
	int_iic_start();
	int_iic_write(devaddr);
	int_iic_write(addr>>8);
	int_iic_write(addr);
	int_iic_write(dat);
	int_iic_stop();
	
	terminal_add_bottom("OK");
	return COMMAND_OK;
}