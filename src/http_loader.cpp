#include "http_loader.h"
#include "Madown.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <regex>
#include <stdlib.h>
#include <streambuf>
#include <string>

http_loader::http_loader() {
    m_curl = curl_easy_init();
    curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(m_curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(m_curl, CURLOPT_TCP_KEEPIDLE, 120L);
    curl_easy_setopt(m_curl, CURLOPT_TCP_KEEPINTVL, 60L);
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION,
                     http_loader::curlWriteFunction);
}

http_loader::~http_loader() { curl_easy_cleanup(m_curl); }

size_t http_loader::curlWriteFunction(void *ptr, size_t size, size_t nmemb,
                                      void *stream) {
    Glib::RefPtr<Gio::MemoryInputStream> *s =
        (Glib::RefPtr<Gio::MemoryInputStream> *)stream;
    (*s)->add_data(ptr, size * nmemb);
    return size * nmemb;
}

static std::string loadfile(const char *file, void *stream) {
    std::string str;
    std::string f(file);
    //bool endwith = f.compare(f.size() - 3, 3, ".md") == 0;
    //if (endwith) {
    //    md::IO::FileStream fs(file);

        //md::Madown madown;
        //str = madown.render(fs);
    //} else {
        std::ifstream t(file);
        if (t) {
            // std::string str((std::istreambuf_iterator<char>(t)),
            //                std::istreambuf_iterator<char>());
            t.seekg(0, std::ios::end);
            str.reserve(t.tellg());
            t.seekg(0, std::ios::beg);
            str.assign((std::istreambuf_iterator<char>(t)),
                       std::istreambuf_iterator<char>());
        }
    //}
    if (str.size() > 0) {
        Glib::RefPtr<Gio::MemoryInputStream> *s =
            (Glib::RefPtr<Gio::MemoryInputStream> *)stream;
        (*s)->add_data(str.c_str(), str.length());
        return std::move(str);
    }
    return "";
}

Glib::RefPtr<Gio::InputStream>
http_loader::load_file(const litehtml::tstring &url) {
    m_url = url;

    Glib::RefPtr<Gio::MemoryInputStream> stream =
        Gio::MemoryInputStream::create();

    if (m_curl) {
        std::regex url_regex(
            "^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?",
            std::regex::extended);
        std::smatch url_match_result;
        if (std::regex_match(url, url_match_result, url_regex)) {

            if (url_match_result[2] == "file" || url_match_result[2] == "") {
                loadfile(url.c_str(), &stream);
            } else {
                curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
                curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &stream);
                curl_easy_perform(m_curl);
                char *new_url = NULL;
                if (curl_easy_getinfo(m_curl, CURLINFO_EFFECTIVE_URL,
                                      &new_url) == CURLE_OK) {
                    if (new_url) {
                        m_url = new_url;
                    }
                }
            }
        }
    }

    return stream;
}
