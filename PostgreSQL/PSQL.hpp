#include <postgresql/libpq-fe.h>
#include "basicclasses.hpp"


class PSQL {
private:
    pg_conn* PGCpool;
public:
    PSQL();
    PSQL(const PSQL&);
};