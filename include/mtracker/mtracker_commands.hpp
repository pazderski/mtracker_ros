#pragma once

#include "robot.h"
#include <stdint.h>

class MTrackerCommands  
{
    static constexpr uint16_t cmdBufSize = 128;

    static constexpr uint16_t modeMotorOff = 0x00;
    static constexpr uint16_t modeMotorOn = 0x03;
    static constexpr uint16_t modeSetOdometry = 0x04;

    #pragma pack(1)
    struct CmdFrame
    {
        uint8_t header;
        uint8_t size;
        uint8_t groupAddress;
        uint8_t cmd;
        uint16_t number;
        uint8_t data[cmdBufSize-6];
    };

    #pragma pack(1)
    struct CmdSetVelocity
    {
        int16_t vl;
        int16_t vr;
        static const uint8_t cmd = 1;
    };

    #pragma pack(1)
    struct CmdSetVelocityAndOdometry
    {
        uint16_t status;
        int16_t wl;
        int16_t wr;
        float x;
        float y;
        float th;
        static const uint8_t cmd = 3;
    };

    #pragma pack(1)
    struct CmdAnsSetVelocityAndOdometry
    {
        uint16_t status;
        int16_t wl;
        int16_t wr;
        float x;
        float y;
        float th;
        int16_t ul;
        int16_t ur;
    };

    CmdFrame * outFrame;

    uint8_t groupAddress;		// robot address
    uint8_t request;

    void SetWheelsVelocitiesWithOdometry(uint8_t robot_nr, uint16_t status, double wl, double wr, double x, double y, double th);

public:
    int MakeFrameToSend();
    MTrackerState state;

    int	cmdSent, cmdRead;

    uint8_t bufOut[cmdBufSize];               // buffer for sending
	
    MTrackerCommands();
    virtual ~MTrackerCommands();
	
    int ReadData(uint8_t * s);                  // read data from the buffer

    void SetWheelsVelocities(uint8_t robotNumber, double wl, double wr)
    {
        SetWheelsVelocitiesWithOdometry(robotNumber, modeMotorOn, wl, wr, 0, 0, 0);
    }

    void SwitchOffMotors(uint8_t robotNumber)
    {
        SetWheelsVelocitiesWithOdometry(robotNumber, modeMotorOff, 0, 0, 0, 0, 0);
    }

};

