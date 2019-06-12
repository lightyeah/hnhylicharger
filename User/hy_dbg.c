
#include "hy_instance.h"
#include "hy_dbg.h"
#include "hy_systime.h"

void arch_printf_time(void){
	hy_systime_t *systime = hy_get_systime_handle();
	uint32_t hour=systime->hour_24;
	uint32_t min=systime->minute_60;
	uint32_t sec=systime->second_60;
	arch_printf("%02d:%02d:%02d||",hour,min,sec);
}
void arch_printf_end(void)
{
	arch_printf("\r\n");

}
void arch_dump_hex(const void *data, int len,const char *tips)
{
    int i;
	char dump_buf[128]={0};
	int  dump_size=0;
	const unsigned char *data_p = data;

    if (tips) {
    	arch_printf("%s: size=%d\r\n",tips, len);
    }

    for (i = 0; i < len; i++) {
		if (0 == ((i+1) % 16)) {
			dump_size += sprintf(dump_buf+dump_size, "0x%02x,\r\n", data_p[i]);
			dump_buf[dump_size] = '\0';
			arch_printf("%s",dump_buf);
			dump_size=0;
			continue;
		}
		else{
			dump_size += sprintf(dump_buf+dump_size, "0x%02x,", data_p[i]);
		}
	}

    if(dump_size)
    	arch_printf("%s",dump_buf);

    arch_printf_end();

	return;
}