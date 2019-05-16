/*****************************************************************************

************************************************************************/

#include "GUI.h"
#include "LCD_driver.h"

#define key_time 700



/*------------------首屏显示--------------------------*/
void ceshi()
{
 write_com(0x01);//清除显示，并且设定地址指针为00H
 delay(5);
 
 write_com(0x80);//第一行（如果是地址是：80H，即LCD的第一行的第一个位置显示）
 hzkdis(" www.lcmlcd.com"); 

 write_com(0x90);//第二行（如果是地址是：90H，即LCD的第二行的第一个位置显示）
 hzkdis("人非聖賢孰能無過");
 
 write_com(0x88);//第三行（如果是地址是：88H，即LCD的第二行的第一个位置显示）
 hzkdis("LCD12864点阵液晶");
 
 write_com(0x9A);//第四行（如果是地址是：98H，即LCD的第二行的第一个位置显示）
 hzkdis("测试程序");
 
}

/*------------------清屏命令--------------------------*/  
 void lcd_clear()
 { 
   write_com(0x01);
   delay(10);
 }
   
/*------------------清整个GDRAM空间----------------------------*/
void clrgdram()
{ 
       uchar x,y;
        for(y=0;y<64;y++)
         for(x=0;x<16;x++)
         { 
            write_com(0x34);
            write_com(y+0x80);        //行地址
            write_com(x+0x80);     //列地址
            write_com(0x30);
            write_data(0x00);
            write_data(0x00);
                              
         }
}
/*------------------整屏写入数据:全亮，或者竖条-----------------------------*/
void lcdfill(uchar disdata)
{ 
      uchar x,y;
       for(y=0;y<32;y++)
        for(x=0;x<16;x++)
         {  write_com(0x36);
            write_com(y+0x80);        //行地址
            write_com(x+0x80);     //列地址
            write_com(0x30);
            write_data(disdata);
            write_data(disdata);
            }
   write_com(0x34);                            
   write_com(0x36);
   }
/*------------------显示横-----------------------------*/
void Disp_H_Line()
{  
   uchar x,y;
   uchar k=0x00;  
     for(y=0;y<32;y++)
      {
        k=~k;
       for(x=0;x<16;x++)
         { 
            write_com(0x36);
            write_com(y+0x80);      //行地址
            write_com(x+0x80);     //列地址
            write_com(0x30);
            write_data(k);
            write_data(k);
           };
         }
   write_com(0x34);                          
   write_com(0x36);
}
/*------------------唐诗《静夜思》--------------------------*/
void gushi_display()
{ 
   write_com(0x30);
   lcd_clear();  
   write_com(0x80+FIRST_ADDR);
   hzkdis("床前明月光，");
   write_com(0x90+FIRST_ADDR);
   hzkdis("疑是地上霜。");
   write_com(0x88+FIRST_ADDR);
   hzkdis("举头望明月，");
   write_com(0x98+FIRST_ADDR);
   hzkdis("低头思故乡。");
   write_com(0xb3+FIRST_ADDR);
   hzkdis("静夜思");
   write_com(0xaa+FIRST_ADDR);
   hzkdis("( 唐 )李白");
}
/*----------在第7列的位置加入两条竖线---------------
void shuxian()
{ 
  uchar x,y;    
    for(x=0;x<9;x+=8) 
       for(y=0;y<32;y++)
          { 
            write_com(0x36);
            write_com(y+0x80);    //行地址
            write_com(x+0x80);     //列地址
            write_com(0x30);
            write_data(0x80);
            write_data(0x00);
          }
for(x=0;x<9;x+=8) 
       for(y=0;y<32;y++)
          { 
            write_com(0x36);
            write_com(y+0x80);    //行地址
            write_com(x+0x90);     //列地址
            write_com(0x30);
            write_data(0x00);
            write_data(0x01);
          }
}    
/*------------------显示边框-------------------------------*/
void display_x_wdian()
{
     uchar x,y;
     uchar i=0;
 for(i=0;i<32;i++)
    {
     for(y=0;y<2;y++)
      {
         for(x=0;x<8;x++)           
           {
            write_com(0x36);
            write_com(y*31+0x80+i);      //行地址
            write_com(x+0x80+8*y);        //列地址
            write_com(0x30);
            write_data(0xaa);
            write_data(0xaa);
           }       
     }
     i+=1;
     for(y=0;y<2;y++)
      {
         for(x=0;x<8;x++)           
           {
            write_com(0x36);
            write_com(y*31+0x80+i);      //行地址
            write_com(x+0x80+8*y);        //列地址
            write_com(0x30);
            write_data(0x55);
            write_data(0x55);
           }       
     }
  }   
 for(i=32;i<64;i++)
    {
     for(y=0;y<2;y++)
      {
         for(x=0;x<8;x++)           
           {
            write_com(0x36);
            write_com(y*31+0x80+i);      //行地址
            write_com(x+0x80+8*y);        //列地址
            write_com(0x30);
            write_data(0x55);
            write_data(0x55);
           }       
     }
     i+=1;
     for(y=0;y<2;y++)
      {
         for(x=0;x<8;x++)           
           {
            write_com(0x36);
            write_com(y*31+0x80+i);      //行地址
            write_com(x+0x80+8*y);        //列地址
            write_com(0x30);
            write_data(0xaa);
            write_data(0xaa);
           }       
     }
  
  }
    for(y=0;y<2;y++)
      {
         for(x=0;x<8;x++)           
           {
            write_com(0x36);
            write_com(y*31+0x80+i);      //行地址
            write_com(x+0x80+8*y);        //列地址
            write_com(0x30);
            write_data(0xff);
            write_data(0xff);
           }       
     }
}
/*******************************************************

*********************************************************/ 
void SSmain(){ 
	LCD_Configuration();
	lcdreset();                    //初始化LCD屏 
  while(1){   
		clrgdram();
    lcd_clear();                  //清屏
    display_x_wdian();                     //显示边框
    delay_ms(1000); 
	 }
}

