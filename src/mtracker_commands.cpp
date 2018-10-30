#include "mtracker_commands.hpp"
#include "crc16.h"

MTrackerCommands::MTrackerCommands()
{
	groupAddress = 1;						// domyœlnie grupa robotow nr 1
	request = 0;
	outFrame = (CmdFrame*) bufOut;
	cmdRead = -1;
}

MTrackerCommands::~MTrackerCommands() {}

/*******************************************************************************************************************************
  Ustawianie predkosci kol oraz odometrii
*******************************************************************************************************************************/
void MTrackerCommands::SetWheelsVelocitiesWithOdometry(uint8_t robot, uint16_t status, double wl, double wr, double x, double y, double th)
{
	auto * data = (CmdSetVelocityAndOdometry *) outFrame->data;
	outFrame->number = robot;
	outFrame->size = sizeof(CmdSetVelocityAndOdometry); 
	outFrame->cmd = data->cmd;
	
	data->status = status;
	data->wl = int16_t(wl*256.0); 
	data->wr = int16_t(wr*256.0); 
	data->x = (float) x;
	data->y = (float) y;
	data->th = (float) th;
	
	MakeFrameToSend();
    request = 1;
    // save command number
    cmdSent = data->cmd;
    // not yet received
    cmdRead = -1;
}

/*******************************************************************************************************************************
  Budowanie ramki do wys³ania zdefniowanego w ramce outFrame
*******************************************************************************************************************************/
int MTrackerCommands::MakeFrameToSend()
{
	uint16_t CRC16_calc = 0;
	
	outFrame->header = 0xAA;
	outFrame->size += 4;
	auto size = outFrame->size;
	outFrame->groupAddress = groupAddress;
	
	CRC16Init();
								
	for(int i = 1; i <= size + 1; i++) 	
		CRC16Calc(bufOut[i]);
	CRC16_calc = CRC16GetValue();

	bufOut[size + 2] = CRC16_calc>>8;
	bufOut[size + 3] = uint8_t(CRC16_calc);

    // compute size of the whole frame
    return size + 4;
}

/*******************************************************************************************************************************
  Read a responce frame (recieved from the robot)
*******************************************************************************************************************************/
int MTrackerCommands::ReadData(unsigned char * s)	// czytaj dane wejœciowe z bufora
{
	auto * frame = (CmdFrame *) s;
	
	if ((frame->cmd == cmdSent) && request)
	{
		switch (frame->cmd)
		{
			case CmdSetVelocityAndOdometry::cmd:
			{
				cmdRead = cmdSent;
				auto * data = (CmdAnsSetVelocityAndOdometry *) frame->data;
				state.wr = (float) data->wr/256.0f;
				state.wl = (float) data->wl/256.0f;
				state.x = (float) data->x;
				state.y = (float) data->y;
				state.th = (float) data->th;
				state.ul = (float) data->ul/4096.0f;
				state.ur = (float) data->ur/4096.0f;

				return frame->cmd;
			}
			break;

            case CmdSetVelocity::cmd:   // old frame - for compatibility
				return frame->cmd;
			break;
		}
	}
	return 0;
}
