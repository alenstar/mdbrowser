#ifndef MDBROWSER_MD__H
#define MDBROWSER_MD__H

#include <map>
#include <iostream>
/* Types of semantic values returned by parsers. */
enum keys { LIST,   /* A generic list of values.  For ordered and bullet lists, see below. */
    RAW,    /* Raw markdown to be processed further */
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
    H1, H2, H3, H4, H5, H6,  /* Code assumes that these are in order. */
    BLOCKQUOTE,
    VERBATIM,
    HTMLBLOCK,
    HRULE,
    REFERENCE,
    NOTE
};

/* Semantic value of a parsing action. */
struct Element {
    int               key;
    struct Element    *children;
    struct Element    *next;
};

typedef struct Element element;

namespace md {
    enum Token {
        TOK_H1 = 0, TOK_H2, TOK_H3, TOK_H4, TOK_H5, TOK_H6,
        TOK_EMPHASIS, TOK_IMAGE, TOK_LINK, TOK_TEXT, TOK_LIST,
        TOK_QUOTE, TOK_CODE, TOK_ITALIC, TOK_PAREN_MID, TOK_PAREN_END,
        TOK_HORIZONTAL_RULE,
        TOK_EOF, TOK_EOL
    };
    const std::string keyChars = "-*#![]()`=+";

    std::map<std::string, Token> tokenTable {
            {"#", TOK_H1},
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
            {"===", TOK_HORIZONTAL_RULE},
            {"---", TOK_HORIZONTAL_RULE}
    };

    bool is_key_char(char c) {
        return keyChars.find(c) != std::string::npos;
    }
}
#endif // MDBROWSER_MD__H
