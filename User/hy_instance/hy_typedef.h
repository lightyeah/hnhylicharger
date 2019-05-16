#ifndef __HY_TYPEDEF_H__
#define __HY_TYPEDEF_H__


typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

#define NELEMENTS(x) 							((unsigned int)(sizeof(x)/sizeof(x[0])))
#define SIZEOF(s, m) 							((unsigned int)(sizeof(((s*)0)->m)))

#define true 1
#define false 0
#endif