#ifndef MDLEXER_H
#define MDLEXER_H

typedef enum {
    MDTK_NONE = -1,
    MDTK_P = 0,
    MDTK_H1,
    MDTK_H2,
    MDTK_H3,
    MDTK_H4,
    MDTK_H5,
    MDTK_H6,
} MDTokenKind;

typedef struct MDToken {
    MDTokenKind kind;
    char *content;
} MDToken;

typedef struct MDTokenArray {
    MDToken *items;
    int len, cap;
} MDTokenArray;

typedef struct MDLexer {
    char *src;
    int srcLen;
    int cursor;
} MDLexer;

MDLexer *mdlexer_new(char *src);
void mdlexer_free(MDLexer *lexer);
MDTokenArray *mdlexer_lex(MDLexer *lexer);
void mdlexer_tokenarray_free(MDTokenArray *arr);

#endif // MDLEXER_H
