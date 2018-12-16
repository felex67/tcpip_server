#include "error.hpp"

namespace errcodes {
    
}
error::error() : errcode(errcodes::ERR_SUCCES) {}
error::error(errcodes::code code) : errcode(code) {}
error::~error() {}
error::operator unsigned short() const { return (unsigned short)errcode; }
