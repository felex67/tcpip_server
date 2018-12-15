#include <sys/types.h>
#include <stddef.h>
#include <string>

#define __cplusplus11

class ServerCore;

typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned int iFlags;
typedef unsigned short DrvLicClass;
typedef unsigned short CarStateClass;
typedef unsigned short OrderState;
typedef unsigned short DriverState;

/* Категории ВУ */
namespace DrvLicClasses {
    static const DrvLicClass A =      0x0000;
    static const DrvLicClass A1 =     0x0001;
    static const DrvLicClass B =      0x0002;
    static const DrvLicClass B1 =     0x0003;
    static const DrvLicClass C =      0x0004;
    static const DrvLicClass C1 =     0x0005;
    static const DrvLicClass D =      0x0006;
    static const DrvLicClass D1 =     0x0007;
    static const DrvLicClass BE =     0x0008;
    static const DrvLicClass CE =     0x0009;
    static const DrvLicClass C1E =    0x000a;
    static const DrvLicClass DE =     0x000b;
    static const DrvLicClass D1E =    0x000c;
    static const DrvLicClass M =      0x000d;
    static const DrvLicClass Tm =     0x000e;
    static const DrvLicClass Tb =     0x000f;
};

/* Класс автомобиля */
namespace CarClasses {
    static const CarStateClass A = 0x0100;     //Особо малый, городской
    static const CarStateClass B = 0x0200;     //Малый
    static const CarStateClass C = 0x0300;     //Первый средний, малый средний
    static const CarStateClass D = 0x0400;     //Средний, второй средний
    static const CarStateClass E = 0x0500;     //Высший средний, бизнес
    static const CarStateClass F = 0x0600;     //Высший, представительский
    static const CarStateClass M = 0x0700;     //Минивэн, универсал
};

/* флаги автомобиля */
namespace CarStateFlags {
    static const CarStateClass Repair =      0x10;    //В ремонте
    static const CarStateClass Rest =        0x20;    //Выходной
    static const CarStateClass TechService = 0x30;    //ТО
    static const CarStateClass PhCtrl =      0x40;    //Фотоконтроль
    static const CarStateClass Ready =       0x50;    //Готов
    static const CarStateClass Working =     0x60;    //Работает
};

/* флаги заказа */
namespace OrderStates {
    static const OrderState Registering =   0x00; //Создание
    static const OrderState WaitForDriver = 0x01; //Ожидание машины
    static const OrderState DriverOnWay =   0x02; //Машина выехала
    static const OrderState CarDelivered =  0x03; //Машина подана
    static const OrderState CliInformed =   0x04; //Заказчик оповещён
    static const OrderState CliCommingOut = 0x05; //Заказчик выходит
    static const OrderState WithClient =    0x06; //Поехали
    static const OrderState WaitInProcess = 0x07; //Ожидание в пути
    static const OrderState InTargetPoint = 0x08; //Прибыли на место
    static const OrderState AllPayed =      0x09; //Расчёт произведён
    static const OrderState Finished =      0x0a; //Выполнен
    static const OrderState AddedToBase =   0x0b; //Сохранён в БД
};

/* флаги водителя */
namespace DriverStates {
    static const DriverState Offline =          0b0000000000000001;
    static const DriverState Online =           0b0000000000000010;
    static const DriverState Waiting =          0b0000000000000100;
    static const DriverState WaitAcceptOrder =  0b0000000000001000;
    static const DriverState OnWayToClient =    0b0000000000010000;
};

/*----------------------------------------------------------------------------------*/

/* GPS */
struct WorldPos {
    double Latitude, Longitude;
    WorldPos();
    WorldPos(const double&, const double&);
    explicit WorldPos(const WorldPos&);
    ~WorldPos();
};

/* Тариф */
namespace TaxRate {
    typedef const unsigned char Rate;
    static Rate CarDelivered        = 0;    //Посадка
    static Rate IncludeKM           = 1;    //Включено км
    static Rate IncludeMin          = 2;    //Включено мин
    static Rate freeWaitDelivered   = 3;    //Бесплатное ожидание при подаче
    static Rate freeWaitOnHeading   = 4;    //Бесплатное ожидание в пути
    static Rate perKMInCity         = 5;    //за 1км в городе
    static Rate perKMOutCity        = 6;    //за 1км вне города
    static Rate perMinInCity        = 7;    //за 1мин в городе
    static Rate perMinOutCity       = 8;    //за 1мин вне города
    static Rate perMinWaitHeading   = 9;    //за 1мин ожидания в пути
    static Rate perMinWaitDeliver   = 10;   //за 1мин ожидания подача
    static Rate RateID              = 11;
    static Rate FreeOpt2            = 12;
    static Rate FreeOpt3            = 13;
    static Rate RateOff             = 14;   //количество опций
};

/* Тариф */
struct Tax {
    unsigned char t[TaxRate::RateOff];
    Tax();
    explicit Tax(const Tax&);
    ~Tax();
    Tax& operator=(const Tax&);
    int operator[](const TaxRate::Rate&) const;
    unsigned char& operator[](const TaxRate::Rate&);
};

/* Пользователь */
class User {
friend ServerCore;
protected:
    WorldPos Position;
    size_t id;
    size_t Deposit;
    float Rating;
    int UserSocket;
    std::string Name;
    std::string Phone;
    std::string Email;
    char blowfish[12];
public:
    User();
    User(const User&);
    ushort setID(const size_t id);
    ushort setWorldPos(const WorldPos&);
    ushort setUserData(const char*, const char*, const char*);
    ushort setUserRating(const float);
    ushort setSocket(const int);
    size_t getID() const;
    WorldPos getWorldPos() const;
    const char** getUserData() const;
    float getUserRating() const;
};

/* Клиент */
class Customer : User {
friend ServerCore;
public:
    Customer();
    Customer(const Customer&);
    ~Customer();
};

/* Таксовая лицензия */
struct TaxiLic {
    size_t Given;       //дата выдачи
    size_t Expires;     //действительно до
    char Serial[12];    //Серия и номер
    char Agent[20];
};

/* Полис ОСАГО */
struct InsOSAGO {
    size_t Given;
    size_t Expires;
    char Serial[14];
    char Agent[20];
};

/* Свидетельство ТС */
struct CarCDI {
    size_t Manufactured;    //Год выпуска
    char Manufacturer[20];  //производитель(марка)
    char Model[20];         //Модель
    char CountrySign[12];   //Гос номер
    char CDI[10];           //СТС серия-номер
    char VIN[20];           //VIN-Код
    char Color[16];         //Цвет
};

struct CarTechInfo {
    size_t LastTS_Date;     //Последнее ТО дата
    size_t LastTS_KM;       //Последнее ТО км
    size_t NextTS_Date;     //Следующее ТО дата
    size_t NextTS_KM;       //Следующее ТО км
    size_t OdometerKM;      //Текущий пробег(одометр)
};

/* Автомобиль */
class Car {
friend ServerCore;
    size_t id;
    CarCDI CDI;
    CarTechInfo TechInfo;
    InsOSAGO OSAGO;
    TaxiLic License;
    //Bit-flags
    CarStateClass StateClass;
public:
    Car();
    Car(const Car&);
    ~Car();
};

/* Корпоративный клиент */
class CorpCustomer {
friend ServerCore;
    char* CompName;
public:
    CorpCustomer();
    explicit CorpCustomer(const CorpCustomer&);
    ~CorpCustomer();
};

/* Водительское удостоверение */
struct DrvLic {
    size_t Experience;      //стаж с
    size_t Expires;         //действительно до
    unsigned short Class;   //класс ВУ
    char Serial[10];        //серия,номер
};

/* Водитель */
class Driver : User {
friend ServerCore;
    Car* pCar;
    DriverState State;
public:
    Driver();
    explicit Driver(const Driver&);
    ~Driver();
};

/* Заказ */
class Order {
friend ServerCore;
    WorldPos Begin;
    WorldPos End;
    size_t id;
    size_t* Client;
    size_t* Driver;
    size_t SuggLength;
    size_t SuggTime;
    size_t RealLength;
    size_t RealTime;
    size_t CalculatedPrice;
    unsigned char Agent;
    bool isFixed;
public:
    Order();
    explicit Order(const Order&);
    ~Order();
};

class tstring {
    char* cstring;
    size_t len;
public:
    tstring(const char* const _cstring)
        : cstring(nullptr)
        , len(0)
    {
        if(_cstring != nullptr) {
            while(_cstring[len++]);
            cstring = new char[len--];
            for(size_t i = 0; i < len; i++) {
                cstring[i] = _cstring[i];
            }
        }
        else {
            cstring = new char[1];
            cstring[len] = 0;
        }
    }
    tstring(const tstring&);
    ~tstring() {
        delete[] cstring;
    }
    const char operator[](const size_t &i) const {
        return (i < len ? cstring[i] : 0);
    }
    char& operator[](const size_t &i) {
        return (i < len ? cstring[i] : cstring[len]);
    }
    operator const char* const() const {
        return cstring;
    }
};