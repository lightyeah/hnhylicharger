/*****************************************************************************

************************************************************************/

#include "GUI.h"
#include "LCD_driver.h"

#define key_time 700



/*------------------������ʾ--------------------------*/
void ceshi()
{
 write_com(0x01);//�����ʾ�������趨��ַָ��Ϊ00H
 delay(5);
 
 write_com(0x80);//��һ�У�����ǵ�ַ�ǣ�80H����LCD�ĵ�һ�еĵ�һ��λ����ʾ��
 hzkdis(" www.lcmlcd.com"); 

 write_com(0x90);//�ڶ��У�����ǵ�ַ�ǣ�90H����LCD�ĵڶ��еĵ�һ��λ����ʾ��
 hzkdis("�˷��}�t���ܟo�^");
 
 write_com(0x88);//�����У�����ǵ�ַ�ǣ�88H����LCD�ĵڶ��еĵ�һ��λ����ʾ��
 hzkdis("LCD12864����Һ��");
 
 write_com(0x9A);//�����У�����ǵ�ַ�ǣ�98H����LCD�ĵڶ��еĵ�һ��λ����ʾ��
 hzkdis("���Գ���");
 
}

/*------------------��������--------------------------*/  
 void lcd_clear()
 { 
   write_com(0x01);
   delay(10);
 }
   
/*------------------������GDRAM�ռ�----------------------------*/
void clrgdram()
{ 
       uchar x,y;
        for(y=0;y<64;y++)
         for(x=0;x<16;x++)
         { 
            write_com(0x34);
            write_com(y+0x80);        //�е�ַ
            write_com(x+0x80);     //�е�ַ
            write_com(0x30);
            write_data(0x00);
            write_data(0x00);
                              
         }
}
/*------------------����д������:ȫ������������-----------------------------*/
void lcdfill(uchar disdata)
{ 
      uchar x,y;
       for(y=0;y<32;y++)
        for(x=0;x<16;x++)
         {  write_com(0x36);
            write_com(y+0x80);        //�е�ַ
            write_com(x+0x80);     //�е�ַ
            write_com(0x30);
            write_data(disdata);
            write_data(disdata);
            }
   write_com(0x34);                            
   write_com(0x36);
   }
/*------------------��ʾ��-----------------------------*/
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
            write_com(y+0x80);      //�е�ַ
            write_com(x+0x80);     //�е�ַ
            write_com(0x30);
            write_data(k);
            write_data(k);
           };
         }
   write_com(0x34);                          
   write_com(0x36);
}
/*------------------��ʫ����ҹ˼��--------------------------*/
void gushi_display()
{ 
   write_com(0x30);
   lcd_clear();  
   write_com(0x80+FIRST_ADDR);
   hzkdis("��ǰ���¹⣬");
   write_com(0x90+FIRST_ADDR);
   hzkdis("���ǵ���˪��");
   write_com(0x88+FIRST_ADDR);
   hzkdis("��ͷ�����£�");
   write_com(0x98+FIRST_ADDR);
   hzkdis("��ͷ˼���硣");
   write_com(0xb3+FIRST_ADDR);
   hzkdis("��ҹ˼");
   write_com(0xaa+FIRST_ADDR);
   hzkdis("( �� )���");
}
/*----------�ڵ�7�е�λ�ü�����������---------------
void shuxian()
{ 
  uchar x,y;    
    for(x=0;x<9;x+=8) 
       for(y=0;y<32;y++)
          { 
            write_com(0x36);
            write_com(y+0x80);    //�е�ַ
            write_com(x+0x80);     //�е�ַ
            write_com(0x30);
            write_data(0x80);
            write_data(0x00);
          }
for(x=0;x<9;x+=8) 
       for(y=0;y<32;y++)
          { 
            write_com(0x36);
            write_com(y+0x80);    //�е�ַ
            write_com(x+0x90);     //�е�ַ
            write_com(0x30);
            write_data(0x00);
            write_data(0x01);
          }
}    
/*------------------��ʾ�߿�-------------------------------*/
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
            write_com(y*31+0x80+i);      //�е�ַ
            write_com(x+0x80+8*y);        //�е�ַ
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
            write_com(y*31+0x80+i);      //�е�ַ
            write_com(x+0x80+8*y);        //�е�ַ
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
            write_com(y*31+0x80+i);      //�е�ַ
            write_com(x+0x80+8*y);        //�е�ַ
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
            write_com(y*31+0x80+i);      //�е�ַ
            write_com(x+0x80+8*y);        //�е�ַ
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
            write_com(y*31+0x80+i);      //�е�ַ
            write_com(x+0x80+8*y);        //�е�ַ
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
	lcdreset();                    //��ʼ��LCD�� 
  while(1){   
		clrgdram();
    lcd_clear();                  //����
    display_x_wdian();                     //��ʾ�߿�
    delay_ms(1000); 
	 }
}

