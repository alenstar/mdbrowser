#include "cmark.h"
#include "cmark_extension_api.h"
#include "config.h"
#include "core-extensions.h"
#include "globals.h"
#include "logdef.h"
#include "memory.h"
#include "node.h"
#include "parser.h"
#include "registry.h"
#include "syntax_extension.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) && !defined(__CYGWIN__)
#include <fcntl.h>
#include <io.h>
#endif

std::string urljoin(const std::string &base, const std::string &relative) {
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

typedef enum {
    FORMAT_NONE,
    FORMAT_HTML,
    FORMAT_XML,
    FORMAT_MAN,
    FORMAT_COMMONMARK,
    FORMAT_PLAINTEXT,
    FORMAT_LATEX
} writer_format;

static bool print_document(cmark_node *document, writer_format writer,
                           int options, int width, cmark_parser *parser) {
    char *result;

    cmark_mem *mem = cmark_get_default_mem_allocator();

    switch (writer) {
    case FORMAT_HTML:
        result = cmark_render_html_with_mem(document, options,
                                            parser->syntax_extensions, mem);
        break;
    case FORMAT_XML:
        result = cmark_render_xml_with_mem(document, options, mem);
        break;
    case FORMAT_MAN:
        result = cmark_render_man_with_mem(document, options, width, mem);
        break;
    case FORMAT_COMMONMARK:
        result =
            cmark_render_commonmark_with_mem(document, options, width, mem);
        break;
    case FORMAT_PLAINTEXT:
        result = cmark_render_plaintext_with_mem(document, options, width, mem);
        break;
    case FORMAT_LATEX:
        result = cmark_render_latex_with_mem(document, options, width, mem);
        break;
    default:
        LOGE("Unknown format %d", writer);
        return false;
    }
    LOGD("%s", result);
    mem->free(result);

    return true;
}

static void print_commonmark_extensions(void) {
    cmark_llist *syntax_extensions;
    cmark_llist *tmp;
    cmark_mem *mem = cmark_get_default_mem_allocator();
    syntax_extensions = cmark_list_syntax_extensions(mem);
    for (tmp = syntax_extensions; tmp; tmp = tmp->next) {
        cmark_syntax_extension *ext = (cmark_syntax_extension *)tmp->data;
        LOGI("%s", ext->name);
    }

    cmark_llist_free(mem, syntax_extensions);
}

int commonmark_parser(const char *buffer, size_t bytes) {
    cmark_parser *parser = NULL;
    cmark_node *document = NULL;
    int width = 0;
    char *unparsed;
    writer_format writer = FORMAT_HTML;
    int options = CMARK_OPT_DEFAULT;
    int res = 1;

    cmark_register_plugin(core_extensions_registration);

// #if defined(_WIN32) && !defined(__CYGWIN__)
//   _setmode(_fileno(stdin), _O_BINARY);
//   _setmode(_fileno(stdout), _O_BINARY);
// #endif

//"--sourcepos") == 0) {
//  options |= CMARK_OPT_SOURCEPOS;
//"--hardbreaks") == 0) {
//  options |= CMARK_OPT_HARDBREAKS;
//"--nobreaks") == 0) {
//  options |= CMARK_OPT_NOBREAKS;
//"--smart") == 0) {
//  options |= CMARK_OPT_SMART;
// "--github-pre-lang") == 0) {
//  options |= CMARK_OPT_GITHUB_PRE_LANG;
//"--safe") == 0) {
//  options |= CMARK_OPT_SAFE;
//"--validate-utf8") == 0) {
//  options |= CMARK_OPT_VALIDATE_UTF8;
//"--liberal-html-tag") == 0) {
//  options |= CMARK_OPT_LIBERAL_HTML_TAG;

#if DEBUG
    parser = cmark_parser_new(options);
#else
    parser =
        cmark_parser_new_with_mem(options, cmark_get_arena_mem_allocator());
#endif

    // cmark_syntax_extension *syntax_extension =
    // cmark_find_syntax_extension(argv[i]);
    // cmark_parser_attach_syntax_extension(parser, syntax_extension);

    cmark_parser_feed(parser, buffer, bytes);
    document = cmark_parser_finish(parser);
    print_document(document, writer, options, width, parser);

#if DEBUG
        if (parser) {
        cmark_parser_free(parser);
    }

    if (document) {
        cmark_node_free(document);
    }
#else
        cmark_arena_reset();
#endif

    cmark_release_plugins();
    return res;
}