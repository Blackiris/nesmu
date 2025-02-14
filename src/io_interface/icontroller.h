#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include <stdint.h>

class IController
{
public:
    IController();

    void set_strobe(const uint8_t& value);
    bool get_next_state();
    virtual void update_strobe_buffer() = 0;

protected:
    uint8_t m_strobe_buffer;
    bool m_strobe_status = false;
};

#endif // ICONTROLLER_H
