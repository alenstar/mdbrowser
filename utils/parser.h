//
// Created by alen on 17-7-15.
//

#ifndef MDBROWSER_PARSER__H
#define MDBROWSER_PARSER__H

#include <iostream>
#include <vector>


class Value {
  public:
    std::string raw() {
        std::string str;
        for (auto cstr : m_content) {
            if (cstr) {
                str.append(cstr);
            }
        }
        return str;
    }

    Value() {
        m_content[0] = NULL;
        m_content[1] = NULL;
        m_content[2] = NULL;
    }
    Value(const char *text, const char *btok = NULL, const char *etok = NULL) {
        if (text) {
            this->m_content[1] = strdup(text);
        }
        if (btok) {
            this->m_content[0] = strdup(btok);
        }
        if (etok) {
            this->m_content[2] = strdup(etok);
        }
    }
    ~Value() {
        for (auto v : this->m_values) {
            delete v;
        }
        for (auto cstr : m_content) {
            if (cstr) {
                free(cstr);
            }
        }
    }
    void append(Value *val) {
        if (val)
            this->m_values.push_back(val);
    }
    void append(const char *text) {
        if (text)
            this->m_values.push_back(new Value(text));
    }
    std::string &render(std::string &out) {
        if(this->m_content[0]) out.append(this->m_content[0]);
        if(this->m_content[1]) out.append(this->m_content[1]);
        for (auto v : this->m_values) {
            std::string tmp;
            out.append(v->render(tmp));
        }
        if(this->m_content[2]) out.append(this->m_content[2]);
        return out;
    }

  private:
    std::vector<Value *> m_values;
    char *m_content[3];
};



std::vector<std::string> string_split(const std::string& src, std::string separate_character);

#endif //MDBROWSER_PARSER__H
