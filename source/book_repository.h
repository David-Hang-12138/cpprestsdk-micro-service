#pragma once

#include <cpprest/http_listener.h>
#include <cpprest/json.h>
// #include <cpprest/mysql.h>
// #include "mysql+++.h"
#include "qtl_mysql.hpp"

using namespace std;
// using namespace daotk::mysql;

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class Book
{
public:
    int id;
    utility::string_t title;
    utility::string_t author;
    int year;

    Book(int _id, utility::string_t _title, utility::string_t _author, int _year)
        : id(_id), title(_title), author(_author), year(_year) {}
};

class BookRepository
{
private:
    // sql::mysql::mysql_connection_config config;
    std::map<pid_t, qtl::mysql::database *> m_mapMysqlConn;
    qtl::mysql::database *getMysqlConn();
    void call3rdApi();
    // qtl::mysql::database m_pMysqlConn;

public:
    BookRepository();
    ~BookRepository();
    std::vector<Book> GetAllBooks();
    Book GetBookById(const utility::string_t &id);
    Book AddBook(Book book);
    Book UpdateBook(const utility::string_t &id, Book book);
    void DeleteBook(const utility::string_t &id);
};