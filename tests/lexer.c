#include "../mdlexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

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
    if (!src) {
        printf("Sorry.\n");
        return 1;
    }

    MDLexer *lexer = mdlexer_new(src);

    MDTokenArray *tokens = mdlexer_lex(lexer);
    if (!tokens) {
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
    if (!f)
        return NULL;

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

    file[idx - 1] = 0;
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
        case MDTK_TEXT: return "text";
        case MDTK_ITALIC_DELIMITER: return "italic";
        case MDTK_BOLD_DELIMITER: return "bold";
        case MDTK_HEADING_DELIMITER: return "heading";
        case MDTK_UL_DELIMITER: return "ul item";
        case MDTK_OL_DELIMITER: return "ol item";
        case MDTK_LINE_BREAK: return "line break";
        case MDTK_IMAGE_DELIMITER: return "image";
        case MDTK_ALT_START: return "alt start";
        case MDTK_LINK_START: return "link start";
        case MDTK_LINK_END: return "link end";
        case MDTK_ALT_END: return "alt end";
    }
    return "?";
}
