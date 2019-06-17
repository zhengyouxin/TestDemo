#include "spi.h"

//*************************
//   SPI1  Initation
//
//*************************
void SPI1_Init()  		//SPI1初始化
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef	 SPI_InitStructure;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1|RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;		//PA5 PA6 PA7	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	 //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);			     
	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);  //拉高
	
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex; //	SPI 设置为双线双向全双工
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;	//设置SPI为主模式
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;	//设置SPI数据大小为8位
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;	//设置SPI默认时钟悬空为高电平
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;	//设置SPI数据捕获在第二个时钟沿
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;	//设置SPI NSS管脚由软件管理
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_16;	//设置SPI 波特率预分频值为 256
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;	//设置SPI 数据从高位传输
	SPI_InitStructure.SPI_CRCPolynomial=7;	//设置SPI CRC值计算的多项式
	SPI_Init(SPI1,&SPI_InitStructure);
		
	SPI_Cmd(SPI1,ENABLE); //使能SPI1

}

//*************************
//   SPI2
//
//*************************
void SPI2_Init()  //SPI2初始化
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef	 SPI_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//打开SPI2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;		//PB13 PB14 PB15	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	 //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);			     
	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //拉高
	
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex; //	SPI 设置为双线双向全双工
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;	//设置SPI为主模式
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;	//设置SPI数据大小为8位
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;	//设置SPI默认时钟悬空为高电平
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;	//设置SPI数据捕获在第二个时钟沿
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;	//设置SPI NSS管脚由软件管理
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_16;	//设置SPI 波特率预分频值为 256
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;	//设置SPI 数据从高位传输
	SPI_InitStructure.SPI_CRCPolynomial=7;	//设置SPI CRC值计算的多项式
	SPI_Init(SPI2,&SPI_InitStructure);
		
	SPI_Cmd(SPI2,ENABLE); //使能SPI2

}

//----------------------------------------------------------------
// SPI读写一个字节
//
u8 SPI_ReadWriteByte(SPIType spiNum,u8 dat)		 //SPI2读写一个字节
{
	u8 t;
	u8 temp8;
	if(spiNum == SPI_PORT1){
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET)	//等待发送缓存器空
		{
			t++;
			if(t>=200)return 0;	//超时返回错误标志	
		}
		SPI_I2S_SendData(SPI1,dat); //发送数据
		t=0;
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET)	//等待接收
		{
			t++;
			if(t>=200)return 0;	//超时返回错误标志	
		}
		temp8 =  SPI_I2S_ReceiveData(SPI1); //返回最近SPI1接收的数据

	}else{	

		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)	//等待发送缓存器空
		{
			t++;
			if(t>=200)return 0;	//超时返回错误标志	
		}
		SPI_I2S_SendData(SPI2,dat); //发送数据
		t=0;
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)	//等待接收
		{
			t++;
			if(t>=200)return 0;	//超时返回错误标志	
		}
		temp8 =  SPI_I2S_ReceiveData(SPI2); //返回最近SPI2接收的数据
	}
	return temp8;
}

void SPI_SetSpeed(SPIType spiNum,u8 SPI_BaudRatePrescaler)	//设置SPI2的速度
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));	
	if(spiNum == SPI_PORT1){
		SPI1->CR1&=0XFFC7;		 //修改BR[2:0]值
		SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
		SPI_Cmd(SPI2,ENABLE);	

	}else{
		SPI2->CR1&=0XFFC7;		 //修改BR[2:0]值
		SPI2->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
		SPI_Cmd(SPI2,ENABLE);	
	}
}


