#pragma once

#include <cstdint>
#include "rs232.h"
#include "mtracker_commands.hpp"

class SerialCommunicator
{
public:
  SerialCommunicator() : portNum(USB0) {}
  ~SerialCommunicator()
  {
    Close();
  }

private:
  int   portNum;
  uint8_t rxData[2048];
  int robotNumber;

public:
  MTrackerCommands cmds;

  void Open()
  {
    if (RS232_OpenComport(portNum, 921600, "8N1") != 0 && portNum <= 21)
    {
      this->portNum++;
      this->Open();
    }
  }

  void Close()
  {
    RS232_CloseComport(portNum);
  }

  void Write()
  {
      auto size = cmds.MakeFrameToSend();
      RS232_SendBuf(portNum, cmds.bufOut, size);
  }

  bool Read()
  {
    auto count = RS232_PollComport(portNum, rxData, 128);
    if (count > 0)
    {
        if (cmds.ReadData(rxData))
            return true;
        else
            return false;
    }
    else
        return false;
  }


};
