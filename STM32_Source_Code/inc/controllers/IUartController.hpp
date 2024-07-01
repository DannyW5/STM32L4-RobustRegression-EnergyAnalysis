#ifndef IUARTCONTROLLER_HPP
#define IUARTCONTROLLER_HPP

#include <string>

class IUartController{
public:
    virtual ~IUartController() = default;

    virtual void Init() = 0;
    virtual void SendData(const std::string& str) = 0;
};

#endif