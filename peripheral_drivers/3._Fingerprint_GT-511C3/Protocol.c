#include "ht32f125x.h"
#include "Protocol.h"
#include "Hardware.h"


devinfo gDeviceInfo;

extern SB_OEM_PKT Command_Packet;



int CalcChkSumOfCmdAckPkt( SB_OEM_PKT* pPkt )
{
	unsigned short wChkSum = 0;
	unsigned char * pBuf = (unsigned char*)pPkt;
	int i;
	
	for(i=0;i<(sizeof(SB_OEM_PKT)-SB_OEM_CHK_SUM_SIZE);i++)
		wChkSum += pBuf[i];

	return wChkSum;
}
int CheckChkSumOfRpsAckPkt( SB_OEM_PKT* pPkt )
{
	unsigned short wChkSum = 0;
	unsigned char * pBuf = (unsigned char*)pPkt;
	int i;

	if(pPkt->Head1 != RESPONSE_START_CODE1 || pPkt->Head2 != RESPONSE_START_CODE2)
		return PKT_HDR_ERR;

	if(pPkt->wDevId != DEVICE_ID)
		return PKT_DEV_ID_ERR;

	for(i=0;i<(sizeof(SB_OEM_PKT)-SB_OEM_CHK_SUM_SIZE);i++)
		wChkSum += pBuf[i];

	if(pPkt->wChkSum != wChkSum)
		return PKT_CHK_SUM_ERR;

	return OK;
}
int CalcChkSumOfDataPkt( unsigned char* pDataPkt, int nSize )
{
	int i;
	unsigned short wChkSum = 0;
	unsigned char* pBuf = (unsigned char*)pDataPkt;

	if(*pBuf != DATA_START_CODE1 || *(pBuf + 1) != DATA_START_CODE2)
		return PKT_HDR_ERR;

	if(*(pBuf + 2) != DEVICE_ID || *(pBuf + 3) != ZERO)
		return PKT_DEV_ID_ERR;
	
	for(i=0;i< nSize - SB_OEM_CHK_SUM_SIZE;i++)
		wChkSum += pBuf[i];

	if(*(pBuf + nSize - SB_OEM_CHK_SUM_SIZE) != (wChkSum & 0xFF) || *(pBuf + nSize - SB_OEM_CHK_SUM_SIZE + 1) != (wChkSum & 0xFF00)>>8)
		return PKT_CHK_SUM_ERR;
	return OK;
}

int Open(int Flag,unsigned char* DataBuffer)
{
	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	int i,j;
	unsigned long tmp = 0;
	/*********Open Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = Flag;
	Command_Packet.wCmd = OPEN;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Open Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;
	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;

	if(Flag)
	{
		rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
		if(rtn == FAIL)
			return FAIL;

		rtn = CalcChkSumOfDataPkt(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
		if(rtn != OK)
			return rtn;
		else
		{
			memcpy(DataBuffer,Command_Packet,SB_OEM_PKT_SIZE);
		}
	}
	return OK;
}

int ChangeBaudRate(unsigned long BAUDRATE)
{
	
	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	unsigned char i;
	unsigned long tmp = 0;
	
 // Command_Packet =(SB_OEM_PKT*) malloc(sizeof(SB_OEM_PKT));
	/*********Change Baudrate Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = BAUDRATE;
	Command_Packet.wCmd = CHANGBAUDRATE;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Change Baudrate Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;

	return OK;
	
}
int Termination()
{
//	SB_OEM_PKT* Command_Packet;
//	unsigned char Buffer[12];
	int rtn;
	unsigned char i;
	unsigned long tmp = 0;
//	Command_Packet = Buffer;
	

	/*********Termination Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = ZERO;
	Command_Packet.wCmd = CLOSE;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;

	/*********Open Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;
	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;

	/*********Change Baudrate Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = 9600;
	Command_Packet.wCmd = CHANGBAUDRATE;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;;
	/*********Change Baudrate Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;
	return OK;
}
int Enroll_Start(int ID)
{
//	SB_OEM_PKT* Command_Packet;
//	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	unsigned char i;
	unsigned long tmp = 0;
//	Command_Packet = Buffer;
	/*********Change Baudrate Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = ID;
	Command_Packet.wCmd = ENROLLSTART;;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Change Baudrate Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;

	return OK;
}

int CaptureFinger(char cFlag)
{
//	SB_OEM_PKT* Command_Packet;
//	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	unsigned char i;
	unsigned long tmp = 0;
//	Command_Packet = Buffer;
	/*********Change Baudrate Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = cFlag;
	Command_Packet.wCmd = CAPTURE_FINGER;;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Change Baudrate Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;

	return OK;
}

int Enroll_nth(char Count)
{
//	SB_OEM_PKT* Command_Packet;
//	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	unsigned char i;
	unsigned long tmp = 0;
//	Command_Packet = Buffer;
	/*********Change Baudrate Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = ZERO;
	Command_Packet.wCmd = ENROLLSTART + Count;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Change Baudrate Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;

	return OK;
}

int RemoveFinger()
{
//	SB_OEM_PKT* Command_Packet;
//	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	unsigned char i;
	unsigned long tmp = 0;
//	Command_Packet = Buffer;
	/*********Change Baudrate Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = ZERO;
	Command_Packet.wCmd = ISPRESSFINGER;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Change Baudrate Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;

	return OK;
}
int GetEnrollCount()
{
//	SB_OEM_PKT* Command_Packet;
//	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	unsigned char i;
	unsigned long tmp = 0;
//	Command_Packet = Buffer;
	/*********Change Baudrate Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = ZERO;
	Command_Packet.wCmd = GETENROLLCOUNT;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Change Baudrate Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;

	return Command_Packet.nParam;
}
int Identify()
{
//	SB_OEM_PKT* Command_Packet;
//	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	unsigned char i;
	unsigned long tmp = 0;
//	Command_Packet = Buffer;
	/*********Change Baudrate Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = ZERO;
	Command_Packet.wCmd = IDENTIFY;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Change Baudrate Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;

	return Command_Packet.nParam;
}

int Verify(int ID)
{
//	SB_OEM_PKT* Command_Packet;
//	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	unsigned char i;
	unsigned long tmp = 0;
//	Command_Packet = Buffer;
	/*********Change Baudrate Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = ID;
	Command_Packet.wCmd = VERIFY;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Change Baudrate Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;

	return Command_Packet.nParam;
}


int DeleteID(unsigned char ID)
{
//	SB_OEM_PKT* Command_Packet;
//	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	unsigned char i;
	int tmp = 0;
//	Command_Packet = (SB_OEM_PKT *)Buffer;
	/*********Change Baudrate Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = ID;
	Command_Packet.wCmd = DELETEID;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Change Baudrate Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;
	
	return OK;
}
int DeleteAll()
{
//	SB_OEM_PKT* Command_Packet;
//	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	unsigned char i;
	unsigned long tmp = 0;
//	Command_Packet = Buffer;
	/*********Change Baudrate Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = ZERO;
	Command_Packet.wCmd = DELETEALL;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Change Baudrate Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;

	return OK;
}
int CheckEnroll(int ID)
{
//	SB_OEM_PKT* Command_Packet;
//	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	unsigned char i;
	unsigned long tmp = 0;
//	Command_Packet = Buffer;
	/*********Change Baudrate Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = ID;
	Command_Packet.wCmd = CHECKENROLLED;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Change Baudrate Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;

	return OK;
}
int LED(unsigned char State)
{
//	SB_OEM_PKT* Command_Packet;
//	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	unsigned char i;
	unsigned long tmp = 0;
//	Command_Packet = Buffer;
	/*********Change Baudrate Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = State;
	Command_Packet.wCmd = CMOSLED;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Change Baudrate Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;

	return OK;
}

int GetRawImage()
{
	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	unsigned char i;
	unsigned long tmp = 0;
	//Command_Packet = &CommandBuff[0];
	/*********Open Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = ZERO;
	Command_Packet.wCmd = GETRAWIMAGE;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Open Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;
	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;
	return OK;
}

int GetTemple(int ID,unsigned char* DataBuffer)
{
	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	int i,j,k;
	unsigned long tmp = 0;
	//Command_Packet = &CommandBuff[0];
	/*********Open Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = ID;
	Command_Packet.wCmd = GETTEMPLATE;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Open Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;
	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;
	/*for(k = 0; k < 512; k += SB_OEM_PKT_SIZE)
	{
		rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
		if(rtn == FAIL)
			return FAIL;
		memcpy((DataBuffer + k),Command_Packet,64);
	} */
  rtn = ReceiveCommand(DataBuffer,SB_OEM_HEADER_SIZE + SB_OEM_DEV_ID_SIZE + SB_OEM_CHK_SUM_SIZE + TEMPLATE_SIZE);
  if(rtn == FAIL)
	  return FAIL;
	rtn = CalcChkSumOfDataPkt(DataBuffer,SB_OEM_HEADER_SIZE + SB_OEM_DEV_ID_SIZE + SB_OEM_CHK_SUM_SIZE + TEMPLATE_SIZE);
	if(rtn != OK)
		return rtn;
	return OK;
}

int SetTemple(int ID,unsigned char* DataBuffer)
{
	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	int i,j,k;
	unsigned long tmp = 0;
	//Command_Packet = &CommandBuff[0];
	/*********Open Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = ID;
	Command_Packet.wCmd = SETTEMPLATE;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Open Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;
	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;
	/*for(k = 0; k < 512; k += 64)
	{
		rtn = SendCommand((unsigned char *)DataBuffer + k,SB_OEM_PKT_SIZE);
		if(rtn == FAIL)
			return FAIL;
	}*/
  rtn = SendCommand((unsigned char *)DataBuffer,SB_OEM_HEADER_SIZE + SB_OEM_DEV_ID_SIZE + SB_OEM_CHK_SUM_SIZE + TEMPLATE_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;
	return OK;
}

int VerifyTemplate(int ID,unsigned char* DataBuffer)
{
	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	int i,j,k;
	unsigned long tmp = 0;
	//(unsigned char*)Command_Packet = &CommandBuff[0];
	/*********Open Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = ID & 0xFF;
	Command_Packet.wCmd = VERIFY_TEMPLATE;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Open Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;
	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;
	for(k = 0; k < 512; k += 64)
	{
		rtn = SendCommand((unsigned char *)DataBuffer + k,SB_OEM_PKT_SIZE);
		if(rtn == FAIL)
			return FAIL;
	}
	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;
	return OK;
}

int IdentifyTemplate(int ID,unsigned char* DataBuffer)
{
	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	int i,j,k;
	unsigned long tmp = 0;
	//(unsigned char*)Command_Packet = &CommandBuff[0];
	/*********Open Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = ID & 0xFF;
	Command_Packet.wCmd = IDENTIFY_TEMPLATE;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Open Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;
	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;
	for(k = 0; k < 512; k += 64)
	{
		rtn = SendCommand((unsigned char *)DataBuffer + k,SB_OEM_PKT_SIZE);
		if(rtn == FAIL)
			return FAIL;
	}
	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;
	return OK;
}

int GetDatabaseStart()
{
//	SB_OEM_PKT* Command_Packet;
//	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	unsigned char i;
	unsigned long tmp = 0;
//	Command_Packet = Buffer;
	/*********Change Baudrate Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = ZERO;
	Command_Packet.wCmd = GETDATABASESTART;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Change Baudrate Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;

	return Command_Packet.nParam;
}

int GetDatabaseEnd()
{
//	SB_OEM_PKT* Command_Packet;
//	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	unsigned char i;
	unsigned long tmp = 0;
//	Command_Packet = Buffer;
	/*********Change Baudrate Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = ZERO;
	Command_Packet.wCmd = GETDATABASEEND;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Change Baudrate Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;

	return Command_Packet.nParam;
}

int UpgradeFirmware()
{
//	SB_OEM_PKT* Command_Packet;
//	unsigned char Buffer[12];
//	unsigned char DataBuff[26];
	int rtn;
	unsigned char i;
	unsigned long tmp = 0;
//	Command_Packet = Buffer;
	/*********Change Baudrate Command***********/
	Command_Packet.Head1 = COMMAND_START_CODE1;
	Command_Packet.Head2 = COMMAND_START_CODE2;
	Command_Packet.wDevId = DEVICE_ID;
	Command_Packet.nParam = ZERO;
	Command_Packet.wCmd = UPGRADEFIRMWARE;
	tmp = CalcChkSumOfCmdAckPkt(&Command_Packet);
	Command_Packet.wChkSum = tmp;
	/*********Change Baudrate Command***********/

	rtn = SendCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	rtn = ReceiveCommand(&Command_Packet.Head1,SB_OEM_PKT_SIZE);
	if(rtn == FAIL)
		return FAIL;

	if(Command_Packet.wCmd != ACK)
		return Command_Packet.nParam;

	rtn = CheckChkSumOfRpsAckPkt(&Command_Packet);
	if(rtn != OK)
		return rtn;

	return Command_Packet.nParam;
}
