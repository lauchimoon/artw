#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "dtree.h"
#include "mdlexer.h"

#define streq(a, b) (strcmp((a), (b)) == 0)
#define fail(s, ...) printf(s); return 1;

char *read_file(char *path);

int main(int argc, char **argv)
{
    if (argc < 2) {
        fail("usage: %s <markdown file>\n", argv[0]);
    }

    DOMTree root = dtree_make();
    Tag html_tag = tag_make(TAGTYPE_ELEMENT, "html");
    Tag body_tag = tag_make(TAGTYPE_ELEMENT, "body");

    root = dtree_insert(root, html_tag);
    dtree_insert(root, body_tag);
    DOMTree body = root->nodes[root->nchild - 1];

    char *src = read_file(argv[1]);
    MDLexer *lexer = mdlexer_new(src);
    MDTokenArray *tokens = mdlexer_lex(lexer);

    // Build DOMTree directly
    // NOTE: this will change in the future
    for (int i = 0; i < tokens->len; ++i) {
        MDToken token = tokens->items[i];
        if (token.kind != MDTK_LINE_BREAK) {
            if (token.kind == MDTK_TEXT) {
                dtree_insert(body, tag_make(TAGTYPE_ELEMENT, "p"));
                DOMTree p = body->nodes[body->nchild - 1];
                dtree_insert(p, tag_make(TAGTYPE_TEXT, token.content));
            } else {
                printf("%s: ", token.content);
                fail("TODO: token not implemented\n");
            }
        }
    }

    mdlexer_tokenarray_free(tokens);
    mdlexer_free(lexer);

    dtree_print(root);
    dtree_free(root);
    free(src);
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
