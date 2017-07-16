#include "utils.h"
#include "logdef.h"

#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

// trim from start
static inline std::string &string_ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &string_rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &string_trim(std::string &s) {
    return string_ltrim(string_rtrim(s));
}

std::string string_replace(std::string& str, const std::string from, const std::string to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return str;
    str.replace(start_pos, from.length(), to);
    return str;
}

std::vector<std::string> string_split(const std::string& src, std::string separate_character)
{
    std::vector<std::string> vector;
    int separate_characterLen = separate_character.size();
    int last_position = 0, index = -1;
    while (-1 != (index = src.find(separate_character, last_position)))
    {
        std::string sss = src.substr(last_position, index - last_position);
        if (sss.size())
            vector.push_back(sss);
        last_position = index + separate_characterLen;
    }
    std::string lastString = src.substr(last_position);
    // if (!lastString.empty()) // keep empty line
    //    if (lastString.size())
            vector.push_back(lastString);
    return vector;
}

std::string string_replace_all(const std::string &src, std::string org_str, std::string rep_str)
{
    std::vector<std::string> delimVec = string_split(src, org_str);
    if (delimVec.size() <= 0){
        return src;
    }
    std::string target("");
    std::vector<std::string>::iterator it = delimVec.begin();
    for (; it != delimVec.end(); ++it)
    {
        target += (*it) + rep_str;
    }
    return target;
}

/*
std::vector<std::string>&& split_line(const char* str, size_t size) {
    std::vector<std::string> lines;
    return std::move(lines);
}
 */

int parser_md(const char* md, size_t size) {
    LOGD("parser_md")
    return 0;
}