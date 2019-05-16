#ifndef _driver_h
#define _driver_h
#include <dat.h>
#define uchar unsigned char
#define uint  unsigned int

sbit RS = P2^0;  
sbit RW = P2^1;
sbit E = P2^2;//22
sbit e1=P3^7;
sbit e2=P3^6;

sbit key=P3^4;
#define Lcd_Bus P0         //MCU P1<------> LCM
#define FIRST_ADDR 0       //定义字符/汉字显示起始位置

uchar code zk[]={
0x08,0x20,0x1c,0x10,0x1c,0x1c,0xff,0x9e,
0x7f,0x1e,0x1c,0x1f,0x3e,0x1f,0x3e,0x1f,
0x77,0x1f,0x41,0x3f,0x00,0x7e,0x00,0xfe,
0x83,0xfc,0x7f,0xf8,0x3f,0xf0,0x0f,0xc0,
};

/*------------------检查忙位-----------------------------*/
void chk_busy()
{ 
   RS=0;
   RW=1;
   e1=e2=0;
   Lcd_Bus=0xff;
   while((Lcd_Bus&0x80)==0x80);
    e1=e2=1;
}
/*------------------延时子程序-----------------------------*/
void delay_key(uint t)
{  
   uchar i;
   while(t--)
   for(i=125;i>0;i--)while(key);
}
void delay(uint t)
{
  while(t--);
}
/*------------------写命令到LCD------------------------------*/
void write_com(unsigned char cmdcode)
{
	chk_busy();
	RS=0;
	RW=0;
	 e1=e2=0;
	Lcd_Bus=cmdcode;
	delay(1);////////////////////在数据写入的时候加入适当的延时
	 e1=e2=1;
	delay(1);
}
/*-------------------写数据到LCD----------------------------*/
void write_data(unsigned char Dispdata)
{
	chk_busy();
	RS=1;
	RW=0;
	 e1=e2=0;;
	Lcd_Bus=Dispdata;
	delay(1);///////////////////在数据写入的时候加入适当的延时
	 e1=e2=1;;
	delay(1);
}
/*------------------初始化LCD屏--------------------------*/
void lcdreset()
{  
   delay(2000);
   write_com(0x30);      //0x30
   delay(10);             //选择基本指令集
   write_com(0x30);       //选择8bit数据流
   delay(5);
   write_com(0x0c);       //开显示(无游标、不反白)
   delay(10);
   write_com(0x01);       //清除显示，并且设定地址指针为00H
   delay(500);
   write_com(0x06);       //指定在资料的读取及写入时，设定游标的移动方向及指定显示的移位
   delay(0);
}
/*------------------自定义一个字符代码(一个星星和月亮)--------------------------*/
void CGRAM()
{    
     int i;
     write_com(0x30);  
     write_com(0x40);
     for(i=0;i<16;i++)
     {
      write_data(zk[i*2]);
      write_data(zk[i*2+1]);
     }
}      
/*------------------显示字符串--------------------------*/
void hzkdis(uchar code *s)
{ 
   while(*s>0)    //*s>0
   {  
      write_data(*s);
      s++;
      delay(50);
   }
}

/*------------------显示图片------------------------*/
void display_img(unsigned char code *img)
{  
   uint j=0;
   uchar x,y,i;
       for(i=0;i<9;i+=8)
       for(y=0;y<32;y++)/*原来 为 y<26 ,上下两个半屏不能正常对接显示，导致显示的图片中间有空隙*/
         for(x=0;x<8;x++)
         { 
            write_com(0x36);//功能设置---8BIT控制界面，扩充指令集
            write_com(y+0x80);        //行地址
            write_com(x+0x80+i);     //列地址
            write_com(0x30);
            write_data(img[j++]);
            write_data(img[j++]);
         }   
           
}
#endif
