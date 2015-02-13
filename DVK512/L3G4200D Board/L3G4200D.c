
#include "L3G4200D.h"
#include  <math.h>    
/*******************************************************************************
* Function Name  : I2C_Configuration
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
 void Delay_1ms( u32 nCnt_1ms )
{	
	bcm2835_delay(nCnt_1ms);
}
 void Delay_1us( u32 nCnt_1ms )
{	
	delayMicroseconds(nCnt_1ms);

}

/*******************************************************************************
* Function Name  : I2C_delay
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void delay_us(uint16_t cnt)
{
	delayMicroseconds(cnt);
}

static void write_buffer(u8 adr,u8 data)
{
	
	
	CS_Enable;	
	
	bcm2835_spi_transfer(adr);//��ַ
  
	bcm2835_spi_transfer(data);
	
  CS_Disable;
	
}
void write_buffer8(u8 adr, u8 *data,uint8_t f)
{
	uint8_t i;
	CS_Enable;	
	
	bcm2835_spi_transfer(adr|0x40);//��ַ ��ַ�Զ���1
  for(i=0;i<f;i++)
	bcm2835_spi_transfer(data[i]);
	
  CS_Disable;
	
}
u8 read_buffer(u8 adr)
{
	u8 temp;
	CS_Enable;	
	
	bcm2835_spi_transfer(adr|0x80);//1000 0000��ȡ��ַ
 
	temp=bcm2835_spi_transfer(0xff);
	
  CS_Disable;
	return temp;
}
void read_buffer8(u8 adr, u8 *data,uint8_t f)
{
	uint8_t i;
	CS_Enable;	
	
	bcm2835_spi_transfer(adr|0xc0);//1100 0000��ȡ��ַ�Զ���1
  for(i=0;i<f;i++)
	data[i]=bcm2835_spi_transfer(0xff);
	
  CS_Disable;
	
}

//************��ʼ��L3G4200D*********************************
void Init_L3G4200D(void)
{ 
	uint8_t data[5]={0xcf,0x01,0x08,0x00,0x02 };
  write_buffer8(CTRL_REG1,data,5);
  

// 	write_buffer(CTRL_REG1,0xcf);delay_us(50);
// 	write_buffer(CTRL_REG2,0x01);delay_us(50);
// 	write_buffer(CTRL_REG3,0x08);delay_us(50);
// 	write_buffer(CTRL_REG4,0x00);delay_us(50);
// 	write_buffer(CTRL_REG5,0x02);delay_us(50);
	

	
}
void READ_L3G4200D(float *T_X,float *T_Y,float *T_Z)//
{	 
	 uint8_t BUF[6]={0};
   uint8_t fs,c1;
  uint8_t i;
   float s;
	
	 //short T_X,T_Y,T_Z;
	 int16_t  buffer[3]={0};

   read_buffer8(OUT_X_L,BUF,6);//��ȡ x y z ������
   
   fs=read_buffer(CTRL_REG4);//��ȡ reg4������
   c1=read_buffer(CTRL_REG1);  //����SPIͨ����û�гɹ�
   //printf("c1=%d \r\n", c1 );


   /*
	 I2C_Read(I2C2,L3G4200_Addr,OUT_X_L,BUF,6);
   I2C_Read(I2C2,L3G4200_Addr,CTRL_REG4,&fs,1);
   */
   switch (fs&0x30)
   {
		 case 0x00:s=8.75;break;
		 case 0x10:s=17.5;break;
		 case 0x20:s=70;break;
		 case 0x30:s=70;break;
		 
   }
	
//  	printf("\r\n---------\r\n");
//    printf("x=%d\n",BUF[0] );
// 		 printf("x=%d\n",BUF[1] );
// 		 
// 		 printf("y=%d\n",BUF[2] );
// 		 printf("y=%d\n",BUF[3] );
// 		 
// 		 printf("z=%d\n",BUF[4] );
// 		 printf("z=%d\n",BUF[5] );
// 		 	printf("\r\n---------\r\n");

	 
	 buffer[0]=((int16_t)BUF[1]<<8)+BUF[0];
   *T_X=(float)(buffer[0])*s/1000;
	 //*T_X=*T_X*8.75/1000;
   //printf("x=%d\n", T_X );
	 buffer[1]=((int16_t)BUF[3]<<8)+BUF[2];
   *T_Y=(float)(buffer[1])*s/1000;//-100;
	 //*T_Y=*T_Y*8.75/1000;
   //printf("y=%d", T_Y );
	 buffer[2]=((int16_t)BUF[5]<<8)+BUF[4];
   *T_Z=(float)(buffer[2])*s/1000;//-100;
   //*T_Z=*T_Z*8.75/1000;
	 //printf("z=%d\n", T_Z );
	 
	//printf("reg1=%d\n", BUF[0] );
	//printf("\r\n");
}



