#include "Engine_Respect_Diag.h" //delshadi
#include "MSG_Engine_Respect_Diag.h"
#include "ArrayTools.h"
#include "OtherHeader\Engine_Respect_Diag_OH.h"
Engine_Respect_Diag::Engine_Respect_Diag(void)
{
}
Engine_Respect_Diag::~Engine_Respect_Diag(void)
{
}
bool Engine_Respect_Diag::VVTPosSelf_Learn(CommandAdorner *Command)

{
	
	byte response[255] = {};
	OBDii.Security_Pass(Command, 1);
	byte SessionFrame[2] = {0x10, 0x03};
	OBDii.Send_Receive(sizeof(SessionFrame), SessionFrame, response, sizeof(response));
	
	if(!OnShowMessage_Messages(EngineRespectDiag_Info, Yes_No, true))
		return false;
	
	if(!OnShowMessage_Messages(EngineRespectDiag_Condition, Yes_No, true))
		return false;
	
	if(!OnShowMessage_Messages(EngineRespectDiag_Warning, Yes_No, true))
		return false;
	
	byte Routine1[4] = {0x31, 0x03, 0x01, 0x33};
	if ((OBDii.Send_Receive(sizeof(Routine1), Routine1, response, sizeof(response)) == NULL) || (response[0] == 0x7F)|| (response[4] != 0x02))
	{
		if(!OnShowMessage_Messages(EngineRespectDiag_OperationFailed, Yes_No, true))
			return false;
	}
	else if (response[4]==0x02)
	{
		if(!OnShowMessage_Messages(EngineRespectDiag_OperationSuccess, Yes_No, true))
			return false;
	}
	byte Routine2[4] = {0x31, 0x01, 0x01, 0x33};
	if ((OBDii.Send_Receive(sizeof(Routine2), Routine2, response, sizeof(response)) == NULL) || (response[0] == 0x7F))
		{
			return Error_Handlear();
		}
	byte Routine3[4] = {0x31, 0x02, 0x01, 0x33};
	if ((OBDii.Send_Receive(sizeof(Routine3), Routine3, response, sizeof(response)) == NULL) || (response[0] == 0x7F))
		{
			return Error_Handlear();
		}
	Sleep(500);
	return true;
}				
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Engine_Respect_Diag::ReplaceVGT(CommandAdorner *Command)
{
	byte response[255] = {};
	OBDii.Security_Pass(Command, 1);
	byte SessionFrame[2] = {0x10, 0x03};
	OBDii.Send_Receive(sizeof(SessionFrame), SessionFrame, response, sizeof(response));
	if (OnShowMessage_Messages(EngineRespectDiag_Condition1, Yes_No, true))
		{
		if(OnShowMessage_Messages(EngineRespectDiag_Confirm, Yes_No, true))
			{
			byte Routine1[5] = {0x2F, 0xE9, 0x09, 0x03, 0x00};
			if ((OBDii.Send_Receive(sizeof(Routine1), Routine1, response, sizeof(response)) == NULL) || (response[0] == 0x7F))
			{
				return Error_Handlear();
			}
			Sleep(8000);
			byte Routine2[4] = {0x2F, 0xE9, 0x09, 0x00};
			if ((OBDii.Send_Receive(sizeof(Routine2), Routine2, response, sizeof(response)) == NULL) || (response[0] == 0x7F))
			{
				return Error_Handlear();
			}
			if(!OnShowMessage_Messages(EngineRespectDiag_Successful,Yes_No, true))
				return false;
			return true;
		}
			else 
				return false;
	}
		else
			return false;
		 return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Engine_Respect_Diag::ReplaceECU(CommandAdorner *Command)
{
	byte response[255] = {};
	OBDii.Security_Pass(Command, 1);
	byte SessionFrame[2] = {0x10, 0x03};
	OBDii.Send_Receive(sizeof(SessionFrame), SessionFrame, response, sizeof(response));
	if (OnShowMessage_Messages(EngineRespectDiag_Condition1, Yes_No, true))
	{	
		byte Routine1[4] = {0x31, 0x01, 0xAF, 0x19};
		OBDii.Send_Receive(sizeof(Routine1), Routine1, response, sizeof(response));
		if ((OBDii.Send_Receive(sizeof(Routine1), Routine1, response, sizeof(response)) == NULL) || (response[0] == 0x7F))
		{
			return false;
		}
		if(!OnShowMessage_Messages(EngineRespectDiag_Confirm, Yes_No, true))
			return false;
			Sleep(500);
		if (OnShowMessage_Messages(EngineRespectDiag_Condition2, Yes_No, true))
		{
			byte Routine2[8] = {0x31, 0x01, 0xAF, 0x1A, 0x00, 0x00, 0x00, 0x00};
			OBDii.Send_Receive(sizeof(Routine2), Routine2, response, sizeof(response));
			if ((OBDii.Send_Receive(sizeof(Routine2), Routine2, response, sizeof(response)) == NULL) || (response[0] == 0x7F))
			{
				return false;
			}
			Sleep(500);
			if (OnShowMessage_Messages(EngineRespectDiag_Confirm, Yes_No, true))
			{
				byte Routine3[8] = {0x31, 0x03, 0xAF, 0x1A, 0x00, 0x00, 0x00, 0x00};
				OBDii.Send_Receive(sizeof(Routine3), Routine3, response, sizeof(response));
				if ((OBDii.Send_Receive(sizeof(Routine3), Routine3, response, sizeof(response)) == NULL) || (response[0] == 0x7F))
				{
				if(!OnShowMessage_Messages(EngineRespectDiag_done,Yes_No, true))
					return false;
				return true;
				}
			}
			else
			return false;
			Sleep(500);
		}
		else
		return false;
	}
	else 
		return false;
	 return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Engine_Respect_Diag::delay_exitsuported(u32 delay, char *msg, bool show_timer)
{
	u16 cunt = 10;
	char msg_2[255] = "";
//	byte SessionFrame[] = {0x10, 0x03};
	MSG_Redraw redraw = MSG_Redraw_true;
	while (delay && !Manager.BreakFlag)
	{
		Sleep(100);
		if ((msg || show_timer) && cunt == 10)
		{
			if (msg)
				sprintf(msg_2, "%s\n%s: [%02d : %02d]", (msg == null ? "" : msg), "Remaining Time", (delay / 60000), (delay % 60000) / 1000);
			else
				sprintf(msg_2, "%s: [%02d : %02d]", "Remaining Time", (delay / 60000), (delay % 60000) / 1000);
			OnShowMessage(msg_2, NoneWithoutWait, false, redraw);
			cunt = 0;
		}
		redraw = MSG_Redraw_false;
		cunt++;
		delay -= 100;
	}
	if (Manager.BreakFlag)
	{
		Manager.BreakFlag = false;
		GetLastKey();
		return false;
	}
	Sleep(delay % 100);
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Engine_Respect_Diag::Keyboard_Actuator(CommandAdorner *Command)
{ //int i=0;
	byte response[255] = {};
	OBDii.Security_Pass(Command, 1);
	char MSG[20] = "";
	sprintf(MSG, "0 - %d", (u8)(Command->DB->FaultCodeLength));
	u8 Output = 0;
	if ((Command->DB->FaultCodeLength) == 99)
	{
		OnShowMessage(Messages(EngineRespectDiag_InputRange), None);
		Output = (u8)(GetDecimalDataDialog(MSG, 0, 99)*5/2);
		CommandAdorner::SendingFrame_ByteArrayFrame_structure *sendFrame = Command_SendingFrames(Command, 2);
	    byte send_buff[5] = {sendFrame->ByteFrame[0], sendFrame->ByteFrame[1], sendFrame->ByteFrame[2], sendFrame->ByteFrame[3], Output};
    if ((OBDii.Send_Receive(sizeof(send_buff), send_buff, response, sizeof(response)) == null) || (response[0] == 0x7F))
	  {
		Manager.BreakFlag = true;
		Command->State.IsFaulty = true;
		return true;
		}
	}
	else if ((Command->DB->FaultCodeLength) == 106)
	{
		OnShowMessage(Messages(EngineRespectDiag_InputRange1), None);
		Output = (u8)(GetDecimalDataDialog(MSG, 0, 106)*7/3);
		CommandAdorner::SendingFrame_ByteArrayFrame_structure *sendFrame = Command_SendingFrames(Command, 2);
		byte send_buff[5] = {sendFrame->ByteFrame[0], sendFrame->ByteFrame[1], sendFrame->ByteFrame[2], sendFrame->ByteFrame[3], Output};
		if ((OBDii.Send_Receive(sizeof(send_buff), send_buff, response, sizeof(response)) == null) || (response[0] == 0x7F))
		{
			Manager.BreakFlag = true;
			Command->State.IsFaulty = true;
			return true;
		}
	}
	else if ((Command->DB->FaultCodeLength) == 2550)
	{
		OnShowMessage(Messages(EngineRespectDiag_InputRange2), None);
		Output = (u8)(GetDecimalDataDialog(MSG, 2, 2550)/15);
	CommandAdorner::SendingFrame_ByteArrayFrame_structure *sendFrame = Command_SendingFrames(Command, 2);
	byte send_buff[5] = {sendFrame->ByteFrame[0], sendFrame->ByteFrame[1], sendFrame->ByteFrame[2], sendFrame->ByteFrame[3], Output};
	 if ((OBDii.Send_Receive(sizeof(send_buff), send_buff, response, sizeof(response)) == null) || (response[0] == 0x7F))
	 {
		  Manager.BreakFlag = true;
		  Command->State.IsFaulty = true;
		  return true;
	 }
  }
	delay_exitsuported(5000, null, true);
	OBDii.Send_Receive(Command_SendingFrames(Command, 4)->Lengh, Command_SendingFrames(Command,4)->ByteFrame, response, sizeof(response));
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Engine_Respect_Diag::Error_Handlear(void)
{
	OnShowMessage_Messages(EngineRespectDiag_done,Yes_No, true);
	Sleep(1000);
	return false;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Engine_Respect_Diag::EnginDTC(CommandAdorner* Command)
{
    byte res[1424];
	  OBDii.Security_Pass(Command, 1);
    int res_len = 0;
    memset(res, 0, sizeof(res));
    u16 code = 0;
    byte send_buff[3] = { 0x19, 0x02, 0xFF };
    FaultCodeAdorner _FaultCodeAdorner;
    res_len =  (int)OBDii.Send_Receive(sizeof(send_buff), send_buff, res, sizeof(res));
    if (res_len < 7 || res[0] == 0x7F)
    {
        Manager.BreakFlag = true;
        Command->State.IsFaulty = true;
        return true;
    }
    for (int i = 3; (i + 3) < res_len; i += 4)
    {
			  code = 0;
		    code = (res[i]<<8)  | res[i + 1];  
        if (code == 0xFFFF || code == 0x0000)
					continue;
				
        u16 state = res[i + 3];
        if (_FaultCodeAdorner.CheckFault(code, state, Command))
        {
           if (((state & 0x01) == 0x01)&& (state <= 0x0A))
            {
                _FaultCodeAdorner.AddFault(code,null,GetDTCCategory(code, Command->DB->FaultCodeLength),GetDTCinCategoryMode(code,Command->DB->FaultCodeLength));
            }
        }
    }
    _FaultCodeAdorner.ShowFaults(Command, true);
    Manager.BreakFlag = true;
    return true;
}
