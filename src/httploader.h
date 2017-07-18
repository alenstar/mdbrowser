#pragma once
#include "globals.h"

class HttpLoader
{
    CURL*       m_curl;
    std::string m_url;
public:
    HttpLoader();
    ~HttpLoader();

    Glib::RefPtr< Gio::InputStream > load_file(const litehtml::tstring& url);
    const char* get_url() const;

private:
    static size_t curlWriteFunction( void *ptr, size_t size, size_t nmemb, void *stream );
};

inline const char* HttpLoader::get_url() const
{
    return m_url.c_str();
}
