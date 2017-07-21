//
// Created by alen on 17-7-16.
//

#include "logdef.h"
#include "parser.h"
#include "utils.h"

int main(int argc, const char **argv) {

    if (argc != 1) {
        size_t size = 0;
        const char *filebuf = load_file(argv[1], &size);
        if (filebuf) {
            std::string str(filebuf, size);
            std::vector<std::string> lines = string_split(str, "\r\n");
            for (auto &line : lines) {
                // TODO
                LOGD("%s", line.c_str());
            }

            free((void *)filebuf);
        }
    }
    // Parser parser("# hello1\n## hello2\n1. Golang\n2. Python\n3. Java\n4. Scala\n5. Kotlin");
    // {
    //     std::string out;
    //     parser.render(out);
    //     LOGD("parser: %s", out.c_str());
    // }
    Value v("hello", "<p>", "</p>");
    v.append(new Value("string", "<em>", "</em>"));
    v.append("world");
    std::string str;
    v.render(str);
    LOGD("render: %s", str.c_str());
    return 0;
}