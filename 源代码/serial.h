#ifndef __SERIAL_H__
#define __SERIAL_H__

void serial_init();		//960bps@24.000MHz
void serial_set_timeout(uint16_t _timeout);
void serial2_send(char dat);
char serial2_read();
void serial1_sendstr(uint8_t *str);
void serial2_sendstr(uint8_t *str);
bit serial1_getdata(uint8_t *buffer, uint16_t size);
bit serial2_getdata(uint8_t *buffer, uint16_t size);
void rf_set_rfid(uint16_t rfid);
void rf_set_dvid(uint16_t dvid);
void rf_set_channel(int8_t channel);
void rf_set_power(uint8_t power);
void rf_enable(bit enable);
#define rf_send(chr) serial2_sendstr(chr)
#define rf_read() serial2_read()
#define rf_sendstr(str) serial2_sendstr(str)
#define rf_getdata(buffer,size) serial2_getdata(buffer,size)


#endif