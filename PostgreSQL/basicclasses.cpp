#include <exception>
#include "basicclasses.hpp"

/* GPS */
WorldPos::WorldPos()
    : Latitude(0)
    , Longitude(0)
{}
WorldPos::WorldPos(const double &Lat, const double &Long)
    : Latitude(Lat)
    , Longitude(Long) 
{}
WorldPos::WorldPos(const WorldPos &pos)
    : Latitude(pos.Latitude)
    , Longitude(pos.Longitude)
{}
WorldPos::~WorldPos() {}

/* Тариф */
Tax::Tax() {
    for(unsigned char i = 0; i < TaxRate::RateOff; i++)
        t[i] = 0;
}
//Deep Copy
Tax::Tax(const Tax&) {
    for(unsigned char i = 0; i < TaxRate::RateOff; i++)
        t[i] = 0;
}
Tax::~Tax() {
    for(unsigned char i = 0; i < TaxRate::RateOff; i++)
        t[i] = 0;
}
Tax& Tax::operator=(const Tax &other) {
    if(this != &other) {
        for(unsigned char i = 0; i < TaxRate::RateOff; i++)
            t[i] = 0;
    }
}
int Tax::operator[](const TaxRate::Rate& r) const {
    if(r < TaxRate::RateOff)
        return (int)t[r];
    throw 0;
}
unsigned char& Tax::operator[](const TaxRate::Rate &r) {
    if(r < TaxRate::RateOff)
        return t[r];
    throw 0;
}