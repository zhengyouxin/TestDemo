#include "nrf24L01.h"


const u8 TX_ADDRESS[5]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
const u8 RX_ADDRESS[5]={0x34,0x43,0x10,0x10,0x01};

//��ʼ��24L01��IO��
void NRF24L01_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;


	//ʹ��PA,B,C�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	    	


	GPIO_InitStructure.GPIO_Pin = SPI1_CE_Pin;				// SPI1--CE
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(SPI1_CE_Port, &GPIO_InitStructure);	//��ʼ��ָ��IO
	GPIO_SetBits(SPI1_CE_Port,SPI1_CE_Pin);//����

	GPIO_InitStructure.GPIO_Pin = SPI1_CS_Pin;		// SPI1--CS
 	GPIO_Init(SPI1_CS_Port, &GPIO_InitStructure);	//��ʼ��ָ��IO
 	GPIO_SetBits(SPI1_CS_Port,SPI1_CS_Pin);//����

	GPIO_InitStructure.GPIO_Pin = SPI1_IRQ_Pin;			// SPI1--IRQ
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(SPI1_IRQ_Port, &GPIO_InitStructure);	//��ʼ��ָ��IO
	
	GPIO_InitStructure.GPIO_Pin = SPI2_CE_Pin;	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(SPI2_CE_Port, &GPIO_InitStructure);	//��ʼ��ָ��IO
	GPIO_SetBits(SPI2_CE_Port,SPI2_CE_Pin);//����

	GPIO_InitStructure.GPIO_Pin = SPI2_CS_Pin;	
 	GPIO_Init(SPI2_CS_Port, &GPIO_InitStructure);	//��ʼ��ָ��IO
 	GPIO_SetBits(SPI2_CS_Port,SPI2_CS_Pin);//����

	GPIO_InitStructure.GPIO_Pin = SPI2_IRQ_Pin;	         //SPI2--IQR
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(SPI2_IRQ_Port, &GPIO_InitStructure);	//��ʼ��ָ��IO
 	
						 		 
    SPI2_Init();    		//��ʼ��SPI	
	SPI1_Init();
			 
	SPI1_CE =0; 			//ʹ��24L01
	SPI1_CSN=1;			//SPIƬѡȡ��
	
	SPI2_CE =0; 			//ʹ��24L01
	SPI2_CSN=1;			//SPIƬѡȡ��

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

//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
u8 NRF24L01_Write_Reg(SPIType spiNum,u8 reg,u8 value)
{
	u8 status;	
	if(spiNum == SPI_PORT1){
	   	SPI1_CSN=0;                 //ʹ��SPI����
	  	status =SPI_ReadWriteByte(spiNum,reg);//���ͼĴ����� 
	  	SPI_ReadWriteByte(spiNum,value);      //д��Ĵ�����ֵ
	  	SPI1_CSN=1;                 //��ֹSPI����
	}else{
		SPI2_CSN=0;                 //ʹ��SPI����
	  	status =SPI_ReadWriteByte(spiNum,reg);//���ͼĴ����� 
	  	SPI_ReadWriteByte(spiNum,value);      //д��Ĵ�����ֵ
	  	SPI2_CSN=1;                 //��ֹSPI����
	}
  	return(status);       			//����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(SPIType spiNum,u8 reg)
{
	u8 reg_val;	
	if(spiNum == SPI_PORT1){
	 	SPI1_CSN = 0;          //ʹ��SPI����		
	  	SPI_ReadWriteByte(spiNum,reg);   //���ͼĴ�����
	  	reg_val=SPI_ReadWriteByte(spiNum,0XFF);//��ȡ�Ĵ�������
	  	SPI1_CSN = 1;          //��ֹSPI����
	}else{
		SPI2_CSN = 0;          //ʹ��SPI����		
	  	SPI_ReadWriteByte(spiNum,reg);   //���ͼĴ�����
	  	reg_val=SPI_ReadWriteByte(spiNum,0XFF);//��ȡ�Ĵ�������
	  	SPI2_CSN = 1;          //��ֹSPI����
	}
  	return(reg_val);           //����״ֵ̬
}	

//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
u8 NRF24L01_Read_Buf(SPIType spiNum,u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	 
	if(spiNum == SPI_PORT1){
  		SPI1_CSN = 0;           //ʹ��SPI����
  		status=SPI_ReadWriteByte(spiNum,reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
 		for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI_ReadWriteByte(spiNum,0XFF);//��������
  		SPI1_CSN=1;       //�ر�SPI����
	}else{
		SPI2_CSN = 0;           //ʹ��SPI����
  		status=SPI_ReadWriteByte(spiNum,reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
 		for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI_ReadWriteByte(spiNum,0XFF);//��������
  		SPI2_CSN=1;       //�ر�SPI����

	}
  	return status;        //���ض�����״ֵ̬
}

//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Write_Buf(SPIType spiNum,u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;
	if(spiNum == SPI_PORT1){
	 	SPI1_CSN = 0;          //ʹ��SPI����
	  	status = SPI_ReadWriteByte(spiNum,reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI_ReadWriteByte(spiNum,*pBuf++); //д������	 
	  	SPI1_CSN = 1;       //�ر�SPI����
	}else{
		SPI2_CSN = 0;          //ʹ��SPI����
	  	status = SPI_ReadWriteByte(spiNum,reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI_ReadWriteByte(spiNum,*pBuf++); //д������	 
	  	SPI2_CSN = 1;       //�ر�SPI����
	}
  	return status;          //���ض�����״ֵ̬m
} 

//******************************************************
//
//���24L01�Ƿ����
// spiNum:  SPI_PORT1--SPI1, SPI_PORT2----SPI2
//����ֵ:0���ɹ�;1��ʧ��	
u8 NRF24L01_Check(SPIType spiNum)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
	
	SPI_SetSpeed(spiNum,SPI_BaudRatePrescaler_4); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   	 
	NRF24L01_Write_Buf(spiNum,NRF_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(spiNum,TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//���24L01����	
	return 0;		 //��⵽24L01
}

//*********************************************************
//����NRF24L01����һ������
// spiNum:  SPI_PORT1--SPI1, SPI_PORT2----SPI2
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(SPIType spiNum,u8 *txbuf)
{
	u8 sta;
 	SPI_SetSpeed(spiNum,SPI_BaudRatePrescaler_4);//spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��
 	if(spiNum == SPI_PORT1){	
		SPI1_CE = 0;
	}else{
		SPI2_CE=0;
	}
  	NRF24L01_Write_Buf(spiNum,WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	if(spiNum == SPI_PORT1){	   //��������
		SPI1_CE = 1;
	}else{
		SPI2_CE=1;
	}
 	if(spiNum == SPI_PORT1){		
		while(SPI1_IRQ!=0);//�ȴ��������
 	}else{
		while(SPI2_IRQ!=0);//�ȴ��������
 	}
	sta=NRF24L01_Read_Reg(spiNum,STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX)//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(spiNum,FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(sta&TX_OK)//�������
	{
		return TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
//u8 NRF24L01_RxPacket(SPIType spiNum,u8 *rxbuf)
//{
//	u8 sta;		    							   
//	SPI_SetSpeed(spiNum,SPI_BaudRatePrescaler_8); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
//	sta=NRF24L01_Read_Reg(spiNum,STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
//	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
//	if(sta&RX_OK)//���յ�����
//	{
//		NRF24L01_Read_Buf(spiNum,RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);	//��ȡ����
//		NRF24L01_Write_Reg(spiNum,FLUSH_RX,0xff);	//���RX FIFO�Ĵ���
//		return 0;
//	}	   
//	return 1;//û�յ��κ�����
//}
u8 NRF24L01_RxPacket(SPIType spiNum,u8 *rxbuf)
{
	u8 sta;		    							   
	SPI_SetSpeed(spiNum,SPI_BaudRatePrescaler_8); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	sta=NRF24L01_Read_Reg(spiNum,STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)//���յ�����
	{
		NRF24L01_Read_Buf(spiNum,RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);	//��ȡ����
		NRF24L01_Write_Reg(spiNum,FLUSH_RX,0xff);	//���RX FIFO�Ĵ��� 
		//return 0; 
	}	   
	//return 1;//û�յ��κ�����
	return sta;
}

//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void NRF24L01_RX_Mode(SPIType spiNum)
{
	if(spiNum == SPI_PORT1){
		SPI1_CE =0;
	}else{
		SPI2_CE=0;	
	}
    
	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+SETUP_AW,0x01);//��ַ��� --3�ֽ�
  	NRF24L01_Write_Buf(spiNum,NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ	
	NRF24L01_Write_Buf(spiNum,NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
	  
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  	 
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+RF_CH,42);	     //����RFͨ��Ƶ��
  		 	    
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+RF_SETUP,0x00);//����TX�������,0db����,1Mbps,���������濪��	 
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
//	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 
	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+FEATURF,0x06);		
	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+DYNPD,0x01);	   	/// P0
	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+STATUS,0x70); 		//���TX_DS��MAX_RT�жϱ�־
	NRF24L01_Write_Reg(spiNum,FLUSH_RX,0xff);	//���RX FIFO�Ĵ��� 
    
  	if(spiNum == SPI_PORT1){     //CEΪ��,�������ģʽ
		SPI1_CE =1;
	}else{
		SPI2_CE=1;	
	}  
}					

//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void NRF24L01_TX_Mode(SPIType spiNum)
{														 
	if(spiNum == SPI_PORT1){
		SPI1_CE =0;
	}else{
		SPI2_CE=0;	
	}	   
	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+SETUP_AW,0x01);//��ַ��� --3�ֽ� 
  	NRF24L01_Write_Buf(spiNum,NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
  	NRF24L01_Write_Buf(spiNum,NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+RF_CH,42);       //����RFͨ��Ϊ40
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+RF_SETUP,0x00);  //����TX�������,0db����,1Mbps,���������濪��   
  	NRF24L01_Write_Reg(spiNum,NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	//CEΪ��,10us��������
	if(spiNum == SPI_PORT1){
		SPI1_CE =1;
	}else{
		SPI2_CE=1;	
	}
}




