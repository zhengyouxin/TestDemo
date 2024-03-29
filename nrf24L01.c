#include "nrf24L01.h"


const u8 TX_ADDRESS[5]={0x34,0x43,0x10,0x10,0x01}; //发送地址
const u8 RX_ADDRESS[5]={0x34,0x43,0x10,0x10,0x01};

//初始化24L01的IO口
void NRF24L01_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;


	//使能PA,B,C端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	    	


	GPIO_InitStructure.GPIO_Pin = SPI1_CE_Pin;				// SPI1--CE
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(SPI1_CE_Port, &GPIO_InitStructure);	//初始化指定IO
	GPIO_SetBits(SPI1_CE_Port,SPI1_CE_Pin);//上拉

	GPIO_InitStructure.GPIO_Pin = SPI1_CS_Pin;		// SPI1--CS
 	GPIO_Init(SPI1_CS_Port, &GPIO_InitStructure);	//初始化指定IO
 	GPIO_SetBits(SPI1_CS_Port,SPI1_CS_Pin);//上拉

	GPIO_InitStructure.GPIO_Pin = SPI1_IRQ_Pin;			// SPI1--IRQ
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(SPI1_IRQ_Port, &GPIO_InitStructure);	//初始化指定IO
	
	GPIO_InitStructure.GPIO_Pin = SPI2_CE_Pin;	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(SPI2_CE_Port, &GPIO_InitStructure);	//初始化指定IO
	GPIO_SetBits(SPI2_CE_Port,SPI2_CE_Pin);//上拉

	GPIO_InitStructure.GPIO_Pin = SPI2_CS_Pin;	
 	GPIO_Init(SPI2_CS_Port, &GPIO_InitStructure);	//初始化指定IO
 	GPIO_SetBits(SPI2_CS_Port,SPI2_CS_Pin);//上拉

	GPIO_InitStructure.GPIO_Pin = SPI2_IRQ_Pin;	         //SPI2--IQR
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(SPI2_IRQ_Port, &GPIO_InitStructure);	//初始化指定IO
 	
						 		 
    SPI2_Init();    		//初始化SPI	
	SPI1_Init();
			 
	SPI1_CE =0; 			//使能24L01
	SPI1_CSN=1;			//SPI片选取消
	
	SPI2_CE =0; 			//使能24L01
	SPI2_CSN=1;			//SPI片选取消

}

void NRF24L01_Config(SPIType spiNum)
{
    if(spiNum == SPI_PORT1)
    {
        SPI1_CE = 0;    //chip enable
        SPI1_CSN = 1;   //SPI disable   
    }
    else
    {
        SPI2_CE = 0;    //chip enable
        SPI2_CSN = 1;   //SPI disable
    }
    
}

//SPI写寄存器
//reg:指定寄存器地址
//value:写入的值
u8 NRF24L01_Write_Reg(SPIType spiNum,u8 reg,u8 value)
{
	u8 status;	
	if(spiNum == SPI_PORT1){
	   	SPI1_CSN=0;                 //使能SPI传输
	  	status =SPI_ReadWriteByte(spiNum,reg);//发送寄存器号 
	  	SPI_ReadWriteByte(spiNum,value);      //写入寄存器的值
	  	SPI1_CSN=1;                 //禁止SPI传输
	}else{
		SPI2_CSN=0;                 //使能SPI传输
	  	status =SPI_ReadWriteByte(spiNum,reg);//发送寄存器号 
	  	SPI_ReadWriteByte(spiNum,value);      //写入寄存器的值
	  	SPI2_CSN=1;                 //禁止SPI传输
	}
  	return(status);       			//返回状态值
}
//读取SPI寄存器值
//reg:要读的寄存器
u8 NRF24L01_Read_Reg(SPIType spiNum,u8 reg)
{
	u8 reg_val;	
	if(spiNum == SPI_PORT1){
	 	SPI1_CSN = 0;          //使能SPI传输		
	  	SPI_ReadWriteByte(spiNum,reg);   //发送寄存器号
	  	reg_val=SPI_ReadWriteByte(spiNum,0XFF);//读取寄存器内容
	  	SPI1_CSN = 1;          //禁止SPI传输
	}else{
		SPI2_CSN = 0;          //使能SPI传输		
	  	SPI_ReadWriteByte(spiNum,reg);   //发送寄存器号
	  	reg_val=SPI_ReadWriteByte(spiNum,0XFF);//读取寄存器内容
	  	SPI2_CSN = 1;          //禁止SPI传输
	}
  	return(reg_val);           //返回状态值
}	

//在指定位置读出指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值 
u8 NRF24L01_Read_Buf(SPIType spiNum,u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	 
	if(spiNum == SPI_PORT1){
  		SPI1_CSN = 0;           //使能SPI传输
  		status=SPI_ReadWriteByte(spiNum,reg);//发送寄存器值(位置),并读取状态值   	   
 		for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI_ReadWriteByte(spiNum,0XFF);//读出数据
  		SPI1_CSN=1;       //关闭SPI传输
	}else{
		SPI2_CSN = 0;           //使能SPI传输
  		status=SPI_ReadWriteByte(spiNum,reg);//发送寄存器值(位置),并读取状态值   	   
 		for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI_ReadWriteByte(spiNum,0XFF);//读出数据
  		SPI2_CSN=1;       //关闭SPI传输

	}
  	return status;        //返回读到的状态值
}

//在指定位置写指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Write_Buf(SPIType spiNum,u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;
	if(spiNum == SPI_PORT1){
	 	SPI1_CSN = 0;          //使能SPI传输
	  	status = SPI_ReadWriteByte(spiNum,reg);//发送寄存器值(位置),并读取状态值
	  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI_ReadWriteByte(spiNum,*pBuf++); //写入数据	 
	  	SPI1_CSN = 1;       //关闭SPI传输
	}else{
		SPI2_CSN = 0;          //使能SPI传输
	  	status = SPI_ReadWriteByte(spiNum,reg);//发送寄存器值(位置),并读取状态值
	  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI_ReadWriteByte(spiNum,*pBuf++); //写入数据	 
	  	SPI2_CSN = 1;       //关闭SPI传输
	}
  	return status;          //返回读到的状态值m
} 

//******************************************************
//
//检测24L01是否存在
// spiNum:  SPI_PORT1--SPI1, SPI_PORT2----SPI2
//返回值:0，成功;1，失败	
u8 NRF24L01_Check(SPIType spiNum)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
	
	SPI_SetSpeed(spiNum,SPI_BaudRatePrescaler_4); //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）   	 
	NRF24L01_Write_Buf(spiNum,NRF_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
	NRF24L01_Read_Buf(spiNum,TX_ADDR,buf,5); //读出写入的地址  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//检测24L01错误	
	return 0;		 //检测到24L01
}

//*********************************************************
//启动NRF24L01发送一次数据
// spiNum:  SPI_PORT1--SPI1, SPI_PORT2----SPI2
//txbuf:待发送数据首地址
//返回值:发送完成状况
u8 NRF24L01_TxPacket(SPIType spiNum,u8 *txbuf)
{
	u8 sta;
 	SPI_SetSpeed(spiNum,SPI_BaudRatePrescaler_4);//spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）
 	if(spiNum == SPI_PORT1){	
		SPI1_CE = 0;
	}else{
		SPI2_CE=0;
	}
  	NRF24L01_Write_Buf(spiNum,WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
 	if(spiNum == SPI_PORT1){	   //启动发送
		SPI1_CE = 1;
	}else{
		SPI2_CE=1;
	}
 	if(spiNum == SPI_PORT1){		
		while(SPI1_IRQ!=0);//等待发送完成
 	}else{
		while(SPI2_IRQ!=0);//等待发送完成
 	}
	sta=NRF24L01_Read_Reg(spiNum,STATUS);  //读取状态寄存器的值	   
	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&MAX_TX)//达到最大重发次数
	{
		NRF24L01_Write_Reg(spiNum,FLUSH_TX,0xff);//清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if(sta&TX_OK)//发送完成
	{
		return TX_OK;
	}
	return 0xff;//其他原因发送失败
}

//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:0，接收完成；其他，错误代码
//u8 NRF24L01_RxPacket(SPIType spiNum,u8 *rxbuf)
//{
//	u8 sta;		    							   
//	SPI_SetSpeed(spiNum,SPI_BaudRatePrescaler_8); //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）   
//	sta=NRF24L01_Read_Reg(spiNum,STATUS);  //读取状态寄存器的值    	 
//	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
//	if(sta&RX_OK)//接收到数据
//	{
//		NRF24L01_Read_Buf(spiNum,RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);	//读取数据
//		NRF24L01_Write_Reg(spiNum,FLUSH_RX,0xff);	//清除RX FIFO寄存器
//		return 0;
//	}	   
//	return 1;//没收到任何数据
//}
u8 NRF24L01_RxPacket(SPIType spiNum,u8 *rxbuf)
{
	u8 sta;		    							   
	SPI_SetSpeed(spiNum,SPI_BaudRatePrescaler_8); //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）   
	sta=NRF24L01_Read_Reg(spiNum,STATUS);  //读取状态寄存器的值    	 
	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&RX_OK)//接收到数据
	{
		NRF24L01_Read_Buf(spiNum,RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);	//读取数据
		NRF24L01_Write_Reg(spiNum,FLUSH_RX,0xff);	//清除RX FIFO寄存器 
		//return 0; 
	}	   
	//return 1;//没收到任何数据
	return sta;
}

//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了		   
void NRF24L01_RX_Mode(SPIType spiNum)
{
	if(spiNum == SPI_PORT1){
		SPI1_CE =0;
	}else{
		SPI2_CE=0;	
	}
    
	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+SETUP_AW,0x01);//地址宽度 --3字节
  	NRF24L01_Write_Buf(spiNum,NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址	
	NRF24L01_Write_Buf(spiNum,NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
	  
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+EN_AA,0x01);    //使能通道0的自动应答    
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址  	 
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+RF_CH,42);	     //设置RF通信频率
  		 	    
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+RF_SETUP,0x00);//设置TX发射参数,0db增益,1Mbps,低噪声增益开启	 
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
//	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 
	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+FEATURF,0x06);		
	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+DYNPD,0x01);	   	/// P0
	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+STATUS,0x70); 		//清除TX_DS或MAX_RT中断标志
	NRF24L01_Write_Reg(spiNum,FLUSH_RX,0xff);	//清除RX FIFO寄存器 
    
  	if(spiNum == SPI_PORT1){     //CE为高,进入接收模式
		SPI1_CE =1;
	}else{
		SPI2_CE=1;	
	}  
}					

//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了		   
//CE为高大于10us,则启动发送.	 
void NRF24L01_TX_Mode(SPIType spiNum)
{														 
	if(spiNum == SPI_PORT1){
		SPI1_CE =0;
	}else{
		SPI2_CE=0;	
	}	   
	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+SETUP_AW,0x01);//地址宽度 --3字节 
  	NRF24L01_Write_Buf(spiNum,NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
  	NRF24L01_Write_Buf(spiNum,NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  

  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+RF_CH,42);       //设置RF通道为40
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+RF_SETUP,0x00);  //设置TX发射参数,0db增益,1Mbps,低噪声增益开启   
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	//CE为高,10us后启动发
	if(spiNum == SPI_PORT1){
		SPI1_CE =1;
	}else{
		SPI2_CE=1;	
	}
}




