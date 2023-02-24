#include <vector>
#include <map>
#include <unistd.h>
#include <cpprest/http_client.h>

#include "book_repository.h"

using namespace utility;           // Common utilities like string conversions
using namespace web;               // Common features like URIs.
using namespace web::http;         // Common HTTP functionality
using namespace web::http::client; // HTTP client features

std::vector<Book> BookRepository::GetAllBooks()
{
    std::vector<Book> books;
    // try
    // {
    //     auto res = m_pMysqlConn->query("SELECT id, title, author, year FROM books");
    //     while (!res.eof())
    //     {
    //         int id, year;
    //         std::string title, author;
    //         res.fetch(id, title, author, year);
    //         Book book(id, title, author, year);
    //         books.push_back(book);
    //         res.next();
    //     }
    // }
    // catch (const std::exception &ex)
    // {
    //     std::cout << ex.what() << std::endl;
    // }
    return books;
}

Book BookRepository::GetBookById(const utility::string_t &id)
{
    Book book(0, "", "", 0);
    try
    {
        qtl::mysql::database *pMySqlConn = getMysqlConn();
        pMySqlConn->query(
            "select id, title, author, year from books where id = ?", id, [](const uint32_t id, const std::string &title, const std::string &author, const uint32_t year)
            {
                // std::cout << id << title << author << year << std::endl;
                // book.id = id;
                // book.title = title;
                // book.author = author;
                // book.year = year;
            });
        call3rdApi();
    }
    catch (const std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
    return book;
}

qtl::mysql::database *BookRepository::getMysqlConn()
{
    qtl::mysql::database *pMySqlConn = NULL;
    pid_t tid = gettid();
    if (m_mapMysqlConn.count(tid))
    {
        pMySqlConn = m_mapMysqlConn[tid];
    }
    else
    {
        pMySqlConn = new qtl::mysql::database;
        pMySqlConn->open("localhost", "root", "Test!234", "cppresttest");
        m_mapMysqlConn[tid] = pMySqlConn;
    }
    return pMySqlConn;
}

void BookRepository::call3rdApi()
{
    pplx::create_task([=]()
                      {
                          // empty task
                      });
    return;
    pplx::task<void> requestTask = pplx::create_task([=]()
                                                     {
    try
    {
        // return;
        http_client client(U("http://ip-api.com/"));
        uri_builder builder(U("/json"));
        client.request(methods::GET, builder.to_string())

             // Handle response headers arriving.
             .then([=](http_response response)
                   {
                       //   printf("Received response status code:%u\n", response.status_code());

                       // Write response body into the file.
                       // return response.body().read_to_end(fileStream->streambuf());
                   })
            ;
    }
    catch (const std::exception &e)
    {
        printf("Error exception:%s\n", e.what());
    } });

    try
    {
        requestTask.wait();
    }
    catch (const std::exception &e)
    {
        printf("Error exception:%s\n", e.what());
    }
};

Book BookRepository::AddBook(Book book)
{
    Book ret_book(0, "", "", 0);
    try
    {
        qtl::mysql::database *pMySqlConn = getMysqlConn();
        int id = pMySqlConn->insert("INSERT INTO books (title, author, year) VALUES (?, ?, ?);", forward_as_tuple(book.title, book.author, book.year));
        ret_book.id = id;
        ret_book.title = book.title;
        ret_book.author = book.author;
        ret_book.year = book.year;
        call3rdApi();
    }
    catch (const std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
    return ret_book;
}

Book BookRepository::UpdateBook(const utility::string_t &id, Book book)
{
    try
    {
        qtl::mysql::database *pMySqlConn = getMysqlConn();
        pMySqlConn->execute_direct("update books set title=?, author=?, year=? WHERE id=?", NULL,
                                   book.title, book.author, book.year, id);
        book.id = atoi(id.c_str());
        call3rdApi();
    }
    catch (const std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
    return book;
}

void BookRepository::DeleteBook(const utility::string_t &id)
{
    try
    {
        qtl::mysql::database *pMySqlConn = getMysqlConn();
        pMySqlConn->execute_direct("delete from books where id=?", NULL, id);
        call3rdApi();
    }
    catch (const std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}
BookRepository::BookRepository()
{
}

BookRepository::~BookRepository()
{
    // if (m_pMysqlConn)
    // {
    //     delete m_pMysqlConn;
    //     m_pMysqlConn = NULL;
    // }
}
