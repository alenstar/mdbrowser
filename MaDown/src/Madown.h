#pragma once

#include "Parser.hpp"
#include "Render.h"
#include "TextStream.h"
#include <functional>
#include <iostream>
#include <regex>
#include <string>

using namespace std;

namespace md {
class Madown {
  public:
    string theme;

    Madown(const string &t) : theme(t) {}

    Madown() : theme("base") {}

    template <typename Stream> string render(Stream &s) {
        static const char base_html[] = {
#include "base.html.inc"
            , 0};
        static const char base_css[] = {
#include "base.css.inc"
            , 0};
        string CSS;
        if (!IO::read_str_from_file("../static/" + theme + ".css", CSS)) {
            // std::cout << "fail to load css\n";
            CSS = string(base_css);
        }

        string HTML;
        if (!IO::read_str_from_file("../static/base.html", HTML)) {
            // std::cout << "fail to load HTML\n";
            HTML = string(base_html);
        }

        Parser parser([&s]() -> int { return s.get_char(); });
        Render *render = new Render();

        parser.parse();

        string HTMLBody;
        for (auto &&STNode : parser.article) {
            HTMLBody += STNode->accept(render) + "\n";
        }

        std::regex CSSLabel("\\{%style%\\}");
        std::regex BodyLabel("\\{%body%\\}");

        HTML = std::regex_replace(HTML, CSSLabel, CSS); // insert CSS stylesheet
        HTML = std::regex_replace(HTML, BodyLabel,
                                  HTMLBody); // insert HTML of markdown

        return HTML;
    }
};

} // namespace md
