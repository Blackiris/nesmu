#ifndef CONTROLLER_H
#define CONTROLLER_H

#define BUTTON_A 0b1
#define BUTTON_B 0b10
#define BUTTON_SELECT 0b100
#define BUTTON_START 0b1000
#define BUTTON_UP 0b10000
#define BUTTON_DOWN 0b100000
#define BUTTON_LEFT 0b1000000
#define BUTTON_RIGHT 0b10000000

#include "../io_interface/icontroller.h"

class Controller : public IController
{
public:
    Controller();

    void update_strobe_buffer() override;

};

#endif // CONTROLLER_H
