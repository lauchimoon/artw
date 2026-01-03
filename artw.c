#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "dtree.h"
#include "mdlexer.h"

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
    DOMTree ul = NULL;
    DOMTree ol = NULL;
    bool reading_paragraph = false;
    bool reading_ul = false;
    bool reading_ol = false;
    bool reading_code = false;
    for (int i = 0; i < tokens->len; ++i) {
        MDToken token = tokens->items[i];
        if (token.kind == MDTK_LINE_BREAK) {
            if (!reading_paragraph && !reading_ul && !reading_ol && !reading_code)
                current = NULL;
            else if (tokens->items[i + 1].kind == MDTK_LINE_BREAK && !reading_code) {
                reading_ul = false;
                reading_ol = false;
                current = NULL;
                ++i;
            }

            continue;
        } else if (token.kind == MDTK_HEADING_DELIMITER) {
            reading_paragraph = false;
            reading_ul = false;
            reading_ol = false;
            reading_code = false;
            int hash_count = 0;
            int j = i;
            while (tokens->items[j++].kind == MDTK_HEADING_DELIMITER)
                ++hash_count;

            --j;

            // TODO: if it's not valid, write the text and hashes
            bool has_space = (tokens->items[j].content[0] == ' ');
            if (hash_count <= 6 && has_space) {
                char heading[3] = { 'h', hash_count + '0', '\0' };
                dtree_insert(body, tag_make(TAGTYPE_ELEMENT, heading));
                current = body->nodes[body->nchild - 1];
            }

            i = j - 1;
            continue;
        } else if (token.kind == MDTK_UL_DELIMITER) {
            reading_paragraph = false;
            reading_ol = false;
            reading_code = false;
            if (!reading_ul) {
                dtree_insert(body, tag_make(TAGTYPE_ELEMENT, "ul"));
                ul = body->nodes[body->nchild - 1];
            }

            dtree_insert(ul, tag_make(TAGTYPE_ELEMENT, "li"));
            current = ul->nodes[ul->nchild - 1];
            reading_ul = true;
            continue;
        } else if (token.kind == MDTK_OL_DELIMITER) {
            reading_paragraph = false;
            reading_ul = false;
            reading_code = false;
            if (!reading_ol) {
                dtree_insert(body, tag_make(TAGTYPE_ELEMENT, "ol"));
                ol = body->nodes[body->nchild - 1];
            }

            dtree_insert(ol, tag_make(TAGTYPE_ELEMENT, "li"));
            current = ol->nodes[ol->nchild - 1];
            reading_ol = true;
            continue;
        } else if (token.kind == MDTK_CODEBLOCK) {
            reading_paragraph = false;
            reading_ol = false;
            reading_ul = false;
            if (!reading_code) {
                // Preserve newlines
                dtree_insert(body, tag_make(TAGTYPE_ELEMENT, "pre"));
                DOMTree pre = body->nodes[body->nchild - 1];
                dtree_insert(pre, tag_make(TAGTYPE_ELEMENT, "code"));
                current = pre->nodes[pre->nchild - 1];
            }

            reading_code = !reading_code;
            ++i;
            continue;
        } else if (token.kind == MDTK_ALT_START &&
                    tokens->items[i + 2].kind == MDTK_ALT_END &&
                    tokens->items[i + 3].kind == MDTK_LINK_START &&
                    tokens->items[i + 5].kind == MDTK_LINK_END) {
            char *alt_text = tokens->items[i + 1].content;
            char *url_text = tokens->items[i + 4].content;
            bool is_image = (tokens->items[i - 1].kind == MDTK_IMAGE_DELIMITER);

            if (is_image) {
                Tag image_tag = tag_make(TAGTYPE_ELEMENT, "img");
                tag_add_attr(&image_tag, tag_attr_make(TAG_ATTR_SRC, url_text));
                tag_add_attr(&image_tag, tag_attr_make(TAG_ATTR_ALT, alt_text));
                dtree_insert(current? current : body, image_tag);
            } else {
                if (!current) {
                    dtree_insert(body, tag_make(TAGTYPE_ELEMENT, "p"));
                    current = body->nodes[body->nchild - 1];
                }

                Tag a_tag = tag_make(TAGTYPE_ELEMENT, "a");
                tag_add_attr(&a_tag, tag_attr_make(TAG_ATTR_HREF, url_text));
                dtree_insert(current, a_tag);

                DOMTree a = current->nodes[current->nchild - 1];
                dtree_insert(a, tag_make(TAGTYPE_TEXT, alt_text));
            }

            i += 5;
            continue;
        } else if (token.kind == MDTK_IMAGE_DELIMITER)
            continue;

        if (!current) {
            reading_ul = false;
            reading_ol = false;
            reading_code = false;
            reading_paragraph = true;
            dtree_insert(body, tag_make(TAGTYPE_ELEMENT, "p"));
            current = body->nodes[body->nchild - 1];
        }

        token = tokens->items[i];
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
        } else {
            printf("%s: unknown token '%s'", argv[0], token.content);
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
