#include "../mdlexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

char *read_file(char *path);
void print_token(MDToken token);
char *tokenkind_string(MDTokenKind kind);

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Markdown file to test is required.\n");
        return 1;
    }

    char *src = read_file(argv[1]);
    MDLexer *lexer = mdlexer_new(src);

    MDTokenArray *tokens = mdlexer_lex(lexer);
    if (tokens == NULL) {
        printf("Sorry.\n");
        return 1;
    }

    for (int i = 0; i < tokens->len; i++)
        print_token(tokens->items[i]);

    mdlexer_tokenarray_free(tokens);
    mdlexer_free(lexer);
    return 0;
}

char *read_file(char *path)
{
    FILE *f = fopen(path, "r");
    fseek(f, 0L, SEEK_END);
    size_t filesize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    char *file = calloc(filesize + 1, sizeof(char));
    assert(file != NULL);

#define BUFSIZE 2048
    int idx = 0;
    char c;
    while ((c = fgetc(f)) != EOF)
        file[idx++] = c;

    fclose(f);
    return file;
}

void print_token(MDToken token)
{
    printf("%s: \"%s\"\n", tokenkind_string(token.kind), token.content);
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
        case MDTK_UL_ITEM: return "ul item";
        case MDTK_OL_ITEM: return "ol item";
    }
    return "?";
}
