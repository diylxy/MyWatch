#include "config.h"

//sfr SPSTAT          =   0xcd;                   //SPI状态寄存器
#define SPIF            0x80                    //SPSTAT.7
#define WCOL            0x40                    //SPSTAT.6
//sfr SPCTL           =   0xce;                   //SPI控制寄存器
#define FLASH_CSIG            0x80                    //SPCTL.7
#define SPEN            0x40                    //SPCTL.6
#define DORD            0x20                    //SPCTL.5
#define MSTR            0x10                    //SPCTL.4
#define CPOL            0x08                    //SPCTL.3
#define CPHA            0x04                    //SPCTL.2
#define SPDHH           0x00                    //CPU_CLK/4
#define SPDH            0x01                    //CPU_CLK/16
#define SPDL            0x02                    //CPU_CLK/64
#define SPDLL           0x03                    //CPU_CLK/128
//sfr SPDAT           =   0xcf;                   //SPI数据寄存器

//FLASH_CS
//串行Flash命令集
#define SFC_WREN        0x06                  
#define SFC_WRDI        0x04
#define SFC_RDSR        0x05
#define SFC_WRSR        0x01
#define SFC_READ        0x03
#define SFC_FASTREAD    0x0B
#define SFC_RDID        0xAB
#define SFC_PAGEPROG    0x02
#define SFC_SECTORERASE 0x20
#define SFC_RDCR        0xA1
#define SFC_WRCR        0xF1
#define SFC_SECTORER    0xD7
#define SFC_BLOCKER     0xD8
#define SFC_CHIPER      0xC7


bit g_fFlashOK;                                //Flash状态

bit flash_chk_id();
void flash_init()
{
  SPSTAT = SPIF | WCOL;                       //清除SPI状态
  FLASH_CS = 1;
  SPCTL = FLASH_CSIG | SPEN | MSTR;                 //设置SPI为主模式
	flash_chk_id();
}

uint8_t SpiShift(uint8_t dat)
{
    SPDAT = dat;                                //触发SPI发送
    while (!(SPSTAT & SPIF));                   //等待SPI数据传输完成
    SPSTAT = SPIF | WCOL;                       //清除SPI状态
    
    return SPDAT;
}

/************************************************
检测Flash是否准备就绪
入口参数: 无
出口参数:
    0 : 没有检测到正确的Flash
    1 : Flash准备就绪
************************************************/
bit flash_chk_id()
{
    uint8_t dat1;
    
    FLASH_CS = 0;
    SpiShift(SFC_RDID);                         //发送读取ID命令
    SpiShift(0x00);                             //空读3个字节
    SpiShift(0x00);
    SpiShift(0x00);
    dat1 = SpiShift(0x00);                      //读取制造商ID1
    FLASH_CS = 1;
                                                //检测是否为PM25LVxx系列的Flash
    g_fFlashOK = ((dat1 == 0x17));
    return g_fFlashOK;
}

/************************************************
检测Flash的忙状态
入口参数: 无
出口参数:
    0 : Flash处于空闲状态
    1 : Flash处于忙状态
************************************************/
bit flash_chk_busy()
{
    uint8_t dat;
    
    FLASH_CS = 0;
    SpiShift(SFC_RDSR);                         //发送读取状态命令
    dat = SpiShift(0);                          //读取状态
    FLASH_CS = 1;
    
    return (dat & 0x01);                        //状态值的Bit0即为忙标志
}

/************************************************
使能Flash写命令
入口参数: 无
出口参数: 无
************************************************/
void FlashWriteEnable()
{
    while (flash_chk_busy());                      //Flash忙检测
    FLASH_CS = 0;
    SpiShift(SFC_WREN);                         //发送写使能命令
    FLASH_CS = 1;
}

/************************************************
擦除整片Flash
入口参数: 无
出口参数: 无
************************************************/
void flash_chip_erase()
{
    if (g_fFlashOK)
    {
        FlashWriteEnable();                     //使能Flash写命令
        FLASH_CS = 0;
        SpiShift(SFC_CHIPER);                   //发送片擦除命令
        FLASH_CS = 1;
    }
}

/************************************************
从Flash中读取数据
入口参数:
    addr   : 地址参数
    size   : 数据块大小
    buffer : 缓冲从Flash中读取的数据
出口参数:
    无
************************************************/
void flash_read(uint32_t addr, uint32_t size, uint8_t *buffer)
{
    if (g_fFlashOK)
    {
        while (flash_chk_busy());                  //Flash忙检测
        FLASH_CS = 0;
        SpiShift(SFC_FASTREAD);                 //使用快速读取命令
        SpiShift(((uint8_t *)&addr)[1]);           //设置起始地址
        SpiShift(((uint8_t *)&addr)[2]);
        SpiShift(((uint8_t *)&addr)[3]);
        SpiShift(0);                            //需要空读一个字节
        while (size)
        {
            *buffer = SpiShift(0);              //自动连续读取并保存
            addr++;
            buffer++;
            size--;
        }
        FLASH_CS = 1;
    }
}

/************************************************
写数据到Flash中
入口参数:
    addr   : 地址参数
    size   : 数据块大小
    buffer : 缓冲需要写入Flash的数据
出口参数: 无
************************************************/
void flash_write(uint32_t addr, uint32_t size, uint8_t *buffer)
{
    if (g_fFlashOK)
    while (size)
    {
        FlashWriteEnable();                     //使能Flash写命令
        FLASH_CS = 0;
        SpiShift(SFC_PAGEPROG);                 //发送页编程命令
        SpiShift(((uint8_t *)&addr)[1]);           //设置起始地址
        SpiShift(((uint8_t *)&addr)[2]);
        SpiShift(((uint8_t *)&addr)[3]);
        while (size)
        {
            SpiShift(*buffer);                  //连续页内写
            addr++;
            buffer++;
            size--;
            if ((addr & 0xff) == 0) break;
        }
        FLASH_CS = 1;
    }
}
/************************************************
Flash 扇区擦除
入口参数:
    addr   : 地址参数
出口参数: 无
************************************************/
void flash_erase_sector(uint32_t addr)
{
    if (g_fFlashOK)
    {
			FlashWriteEnable();                     //使能Flash写命令
      FLASH_CS = 0;
      SpiShift(SFC_SECTORERASE);                 //发送页编程命令
      SpiShift(((uint8_t *)&addr)[1]);           //设置起始地址
      SpiShift(((uint8_t *)&addr)[2]);
      SpiShift(((uint8_t *)&addr)[3]);
      FLASH_CS = 1;
    }
}

uint8_t command_flash_program(COMMAND_ARGS)
{
	uint8_t dat[256];
	uint8_t bufferpointer = 0;
	uint32_t addr = 0;
	ES = 0;
	keep_screen_on = 1;
	terminal_add_bottom("Flash Programmer");
	terminal_add_bottom("");
	terminal_add_bottom("");
	//terminal_require_password();
	terminal_add_bottom("Erasing Flash...");
	flash_chip_erase();
	
	while (flash_chk_busy())delay10ms(10);
	terminal_add_bottom("Started");
	delay10ms(100);
	oled_stop_refresh();
	while(KEY_UP == 0);
	RI = TI = 0;
	while(KEY_UP)
	{
		if(RI)
		{
			RI = 0;
			dat[bufferpointer] = SBUF;
			++bufferpointer;
			if(bufferpointer == 0)
			{
				flash_write(addr, 256, dat);
				addr += 256;
				SBUF = 0x01;
				_nop_();
				_nop_();
				while(TI == 0);
				TI = 0;
			}
		}
	}
	keep_screen_on = 0;
	RI = TI = 0;
	ES = 1;
	terminal_add_bottom("Abort");
	sprintf(oled_tmpstr, "%lu", addr);
	terminal_add_bottom(oled_tmpstr);
	return COMMAND_OK;
}