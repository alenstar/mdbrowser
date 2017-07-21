#pragma once

#include <fstream>
#include <string>
#include <cerrno>
#include <clocale>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cairo.h>
#include <gtkmm.h>
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include "litehtml.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <fontconfig/fontconfig.h>
#include <cairo-ft.h>
#include <gdk/gdk.h>
#include <cairomm/context.h>
#include <curl/curl.h>
// #include <Poco/URI.h>

// extern std::string urljoin(const std::string &base, const std::string &relative);

inline std::string urljoin(const std::string &base, const std::string &relative) {
    /*
    try
    {
        Poco::URI uri_base(base);
        Poco::URI uri_res(uri_base, relative);
        return uri_res.toString();
    } catch (...)
    {
        return relative;
    }
     */
    return base + relative;
}

extern bool render_markdown_to_html(const std::string &in, std::string &out);