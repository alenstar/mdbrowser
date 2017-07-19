#ifndef MDBROWSER_MD__H
#define MDBROWSER_MD__H
#include <iostream>
#include <map>
#include <string.h>
#include <vector>
#include <functional>

enum MarkdownToken {
    TOK_H1 = 0,
    TOK_H2,
    TOK_H3,
    TOK_H4,
    TOK_H5,
    TOK_H6,
    TOK_EMPHASIS,
    TOK_IMAGE,
    TOK_LINK,
    TOK_TEXT,
    TOK_LIST,
    TOK_QUOTE,
    TOK_CODE,
    TOK_ITALIC,
    TOK_PAREN_MID,
    TOK_PAREN_END,
    TOK_HORIZONTAL_RULE,
    TOK_NOTE,
    TOK_EOF,
    TOK_EOL
};
const std::string keyChars = "-*#![]()`=+:.";

std::map<std::string, MarkdownToken> tokenTable{{"#", TOK_H1},
                                        {"##", TOK_H2},
                                        {"###", TOK_H3},
                                        {"####", TOK_H4},
                                        {"#####", TOK_H5},
                                        {"######", TOK_H6},
                                        {"**", TOK_EMPHASIS},
                                        {"![", TOK_IMAGE},
                                        {"[", TOK_LINK},
                                        {"](", TOK_PAREN_MID},
                                        {")", TOK_PAREN_END},
                                        {"-", TOK_LIST},
                                        {"+", TOK_LIST},
                                        {"`", TOK_QUOTE},
                                        {"*", TOK_ITALIC},
                                        {"```", TOK_CODE},
                                        {"===", TOK_H1},
                                        {"---", TOK_H2}};

bool is_key_char(char c) { return keyChars.find(c) != std::string::npos; }

/* Types of semantic values returned by parsers. */
enum TokenType {
    LIST, /* A generic list of values.  For ordered and bullet lists, see below.
             */
    RAW,  /* Raw markdown to be processed further */
    SPACE,
    LINEBREAK,
    ELLIPSIS,
    EMDASH,
    ENDASH,
    APOSTROPHE,
    SINGLEQUOTED,
    DOUBLEQUOTED,
    STR,
    LINK,
    IMAGE,
    CODE,
    HTML,
    EMPH,
    STRONG,
    STRIKE,
    PLAIN,
    PARA,
    LISTITEM,
    BULLETLIST,
    ORDEREDLIST,
    H1,
    H2,
    H3,
    H4,
    H5,
    H6, /* Code assumes that these are in order. */
    BLOCKQUOTE,
    VERBATIM,
    HTMLBLOCK,
    HRULE,
    REFERENCE,
    NOTE
};

// template <class T>
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

class Parser {
  public:
    explicit Parser(const char *str, size_t len = 0) : m_text(str, len == 0 ? strlen(str):len), m_text_idx(0) {}
    char next() { 
        char c = '\0';
        if(m_text_idx < m_text.size()) {
            c = m_text[m_text_idx];
            m_text_idx += 1;
        }
        return c; 
    }

    void render(std::string& out) { 
        std::string str;
        char cur = next();
        char nxt = '\0';//next();

        auto next_token = [&cur, &nxt, this](){
            cur = nxt;
            nxt = this->next();
        };

        if(cur == '\0') {
            return;
        }
        
        while (is_key_char(cur) && is_key_char(nxt)) {
            str.append(cur);
            next_token();
        }
        str.append(cur);

        if (tokenTable.find(str) != tokenTable.end()) {
            //return tokenTable[strBuffer];
        } 
    }

  private:
    TokenType m_cur_token;
    unsigned int m_text_idx;
    std::string m_text;
    //std::vector<Value *> m_values;
    Value m_value;
};

/* Semantic value of a parsing action. */
// struct Element {
//     int               key;
//     struct Element    *children;
//     struct Element    *next;
// };

typedef struct Element element;


#endif // MDBROWSER_MD__H
