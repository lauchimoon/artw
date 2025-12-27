#include "../mdlexer.h"
#include <stdio.h>

char *tokenkind_string(MDTokenKind kind);

int main()
{
    char *src = "# Title\nHello world!";
    MDLexer *lexer = mdlexer_new(src);

    MDTokenArray *tokens = mdlexer_lex(lexer);
    if (tokens == NULL) {
        printf("Sorry.\n");
        return 1;
    }

    for (int i = 0; i < tokens->len; i++) {
        printf("%s: \"%s\"\n", tokenkind_string(tokens->items[i].kind), tokens->items[i].content);
    }

    mdlexer_tokenarray_free(tokens);
    mdlexer_free(lexer);
    return 0;
}

char *tokenkind_string(MDTokenKind kind)
{
    switch (kind) {
        case MDTK_P: return "p";
        case MDTK_H1: return "h1";
    }
    return "?";
}
