#ifndef _error_hpp_
#define _error_hpp_
#include "errorcodes.hpp"

class error {
    errcodes::code errcode;
public:
    error();
    error(errcodes::code);
    ~error();
    operator unsigned short() const;
};

#endif //_error_hpp_