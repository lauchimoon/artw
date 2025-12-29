#include "mdlexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#define INIT_CAP 256

char lexer_peek(MDLexer *lexer);

MDTokenArray *tokenarr_init(void);
void tokenarr_append(MDTokenArray *tokens, MDToken token);

bool is_delimiter(char c, bool reading_code);
bool is_reading_codeblock(MDLexer *lexer);
int find_first(char *s, char c);

MDLexer *mdlexer_new(char *src)
{
    MDLexer *lexer = malloc(sizeof(MDLexer));
    assert(lexer != NULL);

    lexer->src = strdup(src);
    lexer->src_len = strlen(lexer->src);
    lexer->cursor = 0;

    return lexer;
}

void mdlexer_free(MDLexer *lexer)
{
    free(lexer->src);
    free(lexer);
}

MDTokenArray *mdlexer_lex(MDLexer *lexer)
{
    MDTokenArray *tokens = tokenarr_init();
    assert(tokens != NULL);
    bool reading_code = false;

    while (lexer->cursor < lexer->src_len) {
        MDToken token;
        char current = lexer->src[lexer->cursor];

        if (current == '\n') {
            token.kind = MDTK_LINE_BREAK;
            token.content = strdup("\\n");
            tokenarr_append(tokens, token);
            ++lexer->cursor;
        } else if (!reading_code && current == '*' && lexer_peek(lexer) == '*') {
            token.kind = MDTK_BOLD_DELIMITER;
            token.content = strdup("**");
            tokenarr_append(tokens, token);
            lexer->cursor += 2;
        } else if (!reading_code && current == '*') {
            token.kind = MDTK_ITALIC_DELIMITER;
            token.content = strdup("*");
            tokenarr_append(tokens, token);
            ++lexer->cursor;
        } else if (!reading_code && current == '#') {
            while (current == '#') {
                token.kind = MDTK_HEADING_DELIMITER;
                token.content = strdup("#");
                tokenarr_append(tokens, token);
                ++lexer->cursor;
                current = lexer->src[lexer->cursor];
            }
        } else if (!reading_code && current == '-') {
            token.kind = MDTK_UL_DELIMITER;
            token.content = strdup("-");
            tokenarr_append(tokens, token);
            ++lexer->cursor;
        } else if (!reading_code && isdigit(current)) {
            int start = lexer->cursor;
            while (isdigit(current)) {
                ++lexer->cursor;
                current = lexer->src[lexer->cursor];
            }

            if (current == '.')
                token.kind = MDTK_OL_DELIMITER;
            else
                token.kind = MDTK_TEXT;

            token.content = strndup(lexer->src + start, lexer->cursor - start);
            tokenarr_append(tokens, token);
            ++lexer->cursor;
        } else if (!reading_code && current == '!') {
            token.kind = MDTK_IMAGE_DELIMITER;
            token.content = strdup("!");
            tokenarr_append(tokens, token);
            ++lexer->cursor;
        } else if (!reading_code && current == '[') {
            token.kind = MDTK_ALT_START;
            token.content = strdup("[");
            tokenarr_append(tokens, token);
            ++lexer->cursor;
        } else if (!reading_code && current == '(') {
            token.kind = MDTK_LINK_START;
            token.content = strdup("(");
            tokenarr_append(tokens, token);
            ++lexer->cursor;
        } else if (!reading_code && current == ')') {
            token.kind = MDTK_LINK_END;
            token.content = strdup(")");
            tokenarr_append(tokens, token);
            ++lexer->cursor;
        } else if (!reading_code && current == ']') {
            token.kind = MDTK_ALT_END;
            token.content = strdup("]");
            tokenarr_append(tokens, token);
            ++lexer->cursor;
        } else if (is_reading_codeblock(lexer)) {
            token.kind = MDTK_CODEBLOCK;
            token.content = strdup("```");
            reading_code = !reading_code;
            tokenarr_append(tokens, token);
            lexer->cursor += 3;
        } else {
            token.kind = MDTK_TEXT;
            int start = lexer->cursor;
            while (!is_delimiter(current, reading_code)) {
                ++lexer->cursor;
                current = lexer->src[lexer->cursor];
            }

            token.content = strndup(lexer->src + start, lexer->cursor - start);
            tokenarr_append(tokens, token);
        }
    }

    return tokens;
}

void mdlexer_tokenarray_free(MDTokenArray *arr)
{
    for (int i = 0; i < arr->len; i++)
        free(arr->items[i].content);

    free(arr->items);
    free(arr);
}

MDTokenArray *tokenarr_init(void)
{
    MDTokenArray *tokens = malloc(sizeof(MDTokenArray));
    assert(tokens != NULL);
    tokens->cap = INIT_CAP;
    tokens->len = 0;
    tokens->items = calloc(tokens->cap, sizeof(MDToken));
    assert(tokens->items != NULL);
    return tokens;
}

void tokenarr_append(MDTokenArray *tokens, MDToken token)
{
    if (tokens->len/2 >= tokens->cap) {
        tokens->cap *= 2;
        tokens->items = realloc(tokens->items, tokens->cap*sizeof(MDToken));
    }

    tokens->items[tokens->len++] = token;
}

char lexer_peek(MDLexer *lexer)
{
    if (lexer->cursor + 1 >= lexer->src_len)
        return '\0';

    return lexer->src[lexer->cursor + 1];
}

bool is_delimiter(char c, bool reading_code)
{
    if (reading_code)
        return c == '\n' || c == '\0';

    return c == '*' || c == '[' || c == ']' || c == '(' || c == ')' ||
        c == '\n' || c == '\0';
}

bool is_reading_codeblock(MDLexer *lexer)
{
    int cursor = lexer->cursor;
    return lexer->src[cursor] == '`' && lexer->src[cursor + 1] == '`' &&
        lexer->src[cursor + 2] == '`';
}

int find_first(char *s, char c)
{
    for (int i = 0; s[i]; ++i)
        if (s[i] == c)
            return i;

    return -1;
}
