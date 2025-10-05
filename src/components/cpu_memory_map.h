#pragma once

#include "../ROM/rom.h"
#include "../io_interface/icontroller.h"

#include "imemory.h"
#include "ram.h"
#include "apu.h"

#define CPU_SPRDMA 0x4014
#define CPU_JOYPAD1 0x4016
#define CPU_JOYPAD2 0x4017

#define APU_PULSE1_CTRL 0x4000
#define APU_PULSE1_RAMPCTRL 0x4001
#define APU_PULSE1_FINETUNE 0x4002
#define APU_PULSE1_COARSETUNE 0x4003

#define APU_PULSE2_CTRL 0x4004
#define APU_PULSE2_RAMPCTRL 0x4005
#define APU_PULSE2_FINETUNE 0x4006
#define APU_PULSE2_COARSETUNE 0x4007

#define APU_TRI_CTRL1 0x4008
#define APU_TRI_CTRL2 0x4009
#define APU_TRI_FREQ1 0x400a
#define APU_TRI_FREQ2 0x400b

#define APU_NOISE_CTRL 0x400c
#define APU_NOISE_FREQ1 0x400e
#define APU_NOISE_FREQ2 0x400f

#define APU_STATUS 0x4015
#define APU_FRAME_COUNTER 0x4017

class CPUMemoryMap : public IMemory
{
public:
    CPUMemoryMap(ROM& rom, RAM& ram, RAM& io_registers,
                 IController* controller1, IController* controller2, APU& apu);

    uint8_t get_value_at(const uint16_t& address) override;
    void set_value_at(const uint16_t& address, const uint8_t& value) override;

private:
    RAM m_rom_banks;
    RAM& m_ram;
    RAM& m_io_registers;
    APU& m_apu;

    IController* m_controller1;
    IController* m_controller2;
};
