#include <cstdio>
#include "basicclasses.hpp"

namespace querries {
    typedef unsigned short ushort;

    const size_t strlen(const char*const cstring) {
        size_t l = 0;
        if(cstring != nullptr) {
            while(cstring[l++]) {
                ++l;
            }
        }
        return l;
    }

    std::string getUserByPhone(const std::string &phone) {
        std::string q = "SELECT (id, name, email, password, lastposLat, lastposLong, deposit) FROM users WHERE phone='" + phone + "';";
        return q;
    };
    std::string getUserByEmail(const std::string &email) {
        std::string q = "SELECT (id, name, phone, password, lastposLat, lastposLong, deposit) FROM users WHERE email='" + email + "';";
        return q;
    }
    std::string updateUserPos(const WorldPos &pos, const size_t &id) {
        const char* str = "UPDATE users SET lastposLat='%0.6f', lastposLong='%0.6f' WHERE id='%d';\n";
        size_t l = querries::strlen(str);
        l += 30;
        char* buffer = new char[l];
        sprintf(buffer, str, pos.Latitude, pos.Longitude, id);
        std::string temp(buffer);
        delete[] buffer;
        return temp;
    }
    
};