#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "dtree.h"
#include "mdlexer.h"

#define streq(a, b) (strcmp((a), (b)) == 0)

char *read_file(const char *path);

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("usage: %s <markdown file> [output file]\n", argv[0]);
        return 1;
    }

    FILE *output_file = (argc < 3)? stdout : fopen(argv[2], "w");

    DOMTree root = dtree_make();
    Tag html_tag = tag_make(TAGTYPE_ELEMENT, "html");
    Tag body_tag = tag_make(TAGTYPE_ELEMENT, "body");

    root = dtree_insert(root, html_tag);
    dtree_insert(root, body_tag);
    DOMTree body = root->nodes[root->nchild - 1];

    const char *src_filename = argv[1];
    char *src = read_file(src_filename);
    if (!src) {
        printf("%s: file '%s' does not exist or is not a Markdown file.\n", argv[0], src_filename);
        return 1;
    }

    MDLexer *lexer = mdlexer_new(src);
    MDTokenArray *tokens = mdlexer_lex(lexer);

    // Build DOMTree
    DOMTree current = NULL;
    for (int i = 0; i < tokens->len; ++i) {
        MDToken token = tokens->items[i];
        if (token.kind == MDTK_LINE_BREAK) {
            current = NULL;
            continue;
        }

        if (!current) {
            switch (token.kind) {
                default:
                    dtree_insert(body, tag_make(TAGTYPE_ELEMENT, "p"));
            }

            if (!current)
                current = body->nodes[body->nchild - 1];
        }

        if (token.kind == MDTK_TEXT)
            dtree_insert(current, tag_make(TAGTYPE_TEXT, token.content));
        else if (token.kind == MDTK_ITALIC_DELIMITER &&
                    tokens->items[i + 2].kind == MDTK_ITALIC_DELIMITER) {
            MDToken text = tokens->items[i + 1];
            dtree_insert(current, tag_make(TAGTYPE_ELEMENT, "i"));
            DOMTree italic = current->nodes[current->nchild - 1];
            dtree_insert(italic, tag_make(TAGTYPE_TEXT, text.content));
            i += 2;
        } else if (token.kind == MDTK_BOLD_DELIMITER &&
                    tokens->items[i + 2].kind == MDTK_BOLD_DELIMITER) {
            MDToken text = tokens->items[i + 1];
            dtree_insert(current, tag_make(TAGTYPE_ELEMENT, "b"));
            DOMTree bold = current->nodes[current->nchild - 1];
            dtree_insert(bold, tag_make(TAGTYPE_TEXT, text.content));
            i += 2;
        }
        else {
            printf("%s: ", token.content);
            printf("TODO: token not implemented\n");
            return 1;
        }
    }

    mdlexer_tokenarray_free(tokens);
    mdlexer_free(lexer);

    dtree_print(root, output_file);
    dtree_free(root);
    free(src);
    return 0;
}

char *read_file(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f)
        return NULL;

    if (!strstr(path, ".md"))
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

    file[idx - 1] = '\0';
    fclose(f);
    return file;
}
