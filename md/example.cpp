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
    return 0;
}