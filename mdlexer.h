#ifndef MDLEXER_H
#define MDLEXER_H

typedef enum {
    MDTK_NONE = -1,
    MDTK_TEXT = 0,
    MDTK_ITALIC_DELIMITER,
    MDTK_BOLD_DELIMITER,
    MDTK_HEADING_DELIMITER,
    MDTK_UL_DELIMITER,
    MDTK_OL_DELIMITER,
    MDTK_LINE_BREAK,
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
    int src_len;
    int cursor;
} MDLexer;

MDLexer *mdlexer_new(char *src);
void mdlexer_free(MDLexer *lexer);
MDTokenArray *mdlexer_lex(MDLexer *lexer);
void mdlexer_tokenarray_free(MDTokenArray *arr);

#endif // MDLEXER_H
