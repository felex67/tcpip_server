#define __cplusplus11

#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <queue>
#include <condition_variable>
#include <postgresql/libpq-fe.h>
#include <thread>
#include <fstream>
#include <sstream>


class PGConnection {

public:
    PGConnection();
    std::shared_ptr<pg_conn> connection() const;

private:
    void establish_connection();

    const std::string m_dbhost = "localhost";
    const int         m_dbport = 5432;
    const std::string m_dbname = "demo";
    const std::string m_dbuser = "demo";
    const std::string m_dbpass = "demo";

    std::shared_ptr<pg_conn>  m_connection;

};

PGConnection::PGConnection() {
    m_connection.reset( PQsetdbLogin(m_dbhost.c_str(), std::to_string(m_dbport).c_str(), nullptr, nullptr, m_dbname.c_str(), m_dbuser.c_str(), m_dbpass.c_str()), &PQfinish );

    if (PQstatus( m_connection.get() ) != CONNECTION_OK && PQsetnonblocking(m_connection.get(), 1) != 0 )
    {
       throw std::runtime_error( PQerrorMessage( m_connection.get() ) );
    }

}


std::shared_ptr<PGconn> PGConnection::connection() const {
    return m_connection;
}

class PGBackend
{
public:
    PGBackend();
    std::shared_ptr<PGConnection> connection();
    void freeConnection(std::shared_ptr<PGConnection>);

private:
    void createPool();
    
    std::mutex m_mutex;
    std::condition_variable m_condition;
    std::queue<std::shared_ptr<PGConnection>> m_pool;

    const int POOL = 10;


};

PGBackend::PGBackend()
{

    createPool();
  
}

void PGBackend::createPool()
{
    std::lock_guard<std::mutex> locker_( m_mutex );

    for ( auto i = 0; i< POOL; ++i ){
         m_pool.emplace ( std::make_shared<PGConnection>() );
    }
}

std::shared_ptr<PGConnection> PGBackend::connection()
{

    std::unique_lock<std::mutex> lock_( m_mutex );

    while ( m_pool.empty() ){
            m_condition.wait( lock_ );
    }

    auto conn_ = m_pool.front();
    m_pool.pop();

    return  conn_;
}


void PGBackend::freeConnection(std::shared_ptr<PGConnection> conn_)
{
    std::unique_lock<std::mutex> lock_( m_mutex );
    m_pool.push( conn_ );
    lock_.unlock();
    m_condition.notify_one();
}
void testConnection(std::shared_ptr<PGBackend> pgbackend)
{
//получаем свободное соединение
    auto conn = pgbackend->connection();

    std::string demo = "SELECT max(id) FROM demo; " ;
    PQsendQuery( conn->connection().get(), demo.c_str() );

    while ( auto res_ = PQgetResult( conn->connection().get()) ) {
        if (PQresultStatus(res_) == PGRES_TUPLES_OK && PQntuples(res_)) {
            auto ID = PQgetvalue (res_ ,0, 0);
            std::cout<< ID<<std::endl;
        }

        if (PQresultStatus(res_) == PGRES_FATAL_ERROR){
            std::cout<< PQresultErrorMessage(res_)<<std::endl;
        }

        PQclear( res_ );
    }

//возвращаем соединение в очередь
    pgbackend->freeConnection(conn);

}


int main(int argc, char const *argv[])
{
	
	auto pgbackend = std::make_shared<PGBackend>();

    
    std::vector<std::shared_ptr<std::thread>> vec;

    for ( size_t i = 0; i< 50 ; ++i ){

        vec.push_back(std::make_shared<std::thread>(std::thread(testConnection, pgbackend)));
    }

    for(auto &i : vec) {
        i.get()->join();
    }

 
    return 0;
}
