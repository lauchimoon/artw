#include "../mdlexer.h"
#include <stdio.h>

char *tokenkind_string(MDTokenKind kind);

int main()
{
    char *src = "# Title\n## Subtitle\n### Subsubtitle\n####### Nothing.\n#Paragraph!\nHello world!\nThis does not leave the same paragraph.\n\nThis does!";
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
        case MDTK_H2: return "h2";
        case MDTK_H3: return "h3";
        case MDTK_H4: return "h4";
        case MDTK_H5: return "h5";
        case MDTK_H6: return "h6";
    }
    return "?";
}
