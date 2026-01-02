#include "dtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_CHILD_MIN 256

DOMTree make_node(Tag tag);
void tagcpy(Tag *dst, Tag src);
void dtree_print_helper(DOMTree t, int level, FILE *out_file);
void indent(int level, FILE *out_file);
const char *attr_type_string(TagAttrType type);

DOMTree dtree_make(void)
{
    return NULL;
}

void dtree_free(DOMTree t)
{
    if (dtree_empty(t))
        return;

    for (int i = 0; i < t->maxchild; ++i)
        dtree_free(t->nodes[i]);

    tag_free(t->tag);
    free(t->tag.content);
    free(t->nodes);
    free(t);
}

int dtree_empty(DOMTree t)
{
    return t == NULL;
}

DOMTree dtree_insert(DOMTree t, Tag tag)
{
    DOMTree node = make_node(tag);
    if (dtree_empty(t)) {
        return node;
    } else {
        if (t->nchild >= t->maxchild) {
            int old_cap = t->maxchild;
            t->maxchild *= 2;
            t->nodes = realloc(t->nodes, t->maxchild*sizeof(DOMTree));
            assert(t->nodes != NULL);
            for (int i = old_cap; i < t->maxchild; ++i)
                t->nodes[i] = NULL;
        }
    }

    t->nodes[t->nchild++] = node;
    return t;
}

void dtree_print(DOMTree t, FILE *out_file)
{
    dtree_print_helper(t, 0, out_file);
}

DOMTree make_node(Tag tag)
{
    DOMTree node = malloc(sizeof(struct DOMTag));
    assert(node != NULL);
    node->tag = tag_make(tag.type, tag.content);
    tagcpy(&node->tag, tag);
    node->nchild = 0;
    node->maxchild = MAX_CHILD_MIN;
    node->nodes = malloc(node->maxchild*sizeof(struct DOMTag));
    assert(node->nodes != NULL);
    for (int i = 0; i < node->maxchild; ++i)
        node->nodes[i] = dtree_make();

    return node;
}

void tagcpy(Tag *dst, Tag src)
{
    dst->type = src.type;
    dst->content = strdup(src.content);
    dst->num_attrs = src.num_attrs;
    for (int i = 0; i < src.num_attrs; ++i) {
        dst->attrs[i].type = src.attrs[i].type;
        dst->attrs[i].value = strdup(src.attrs[i].value);
    }
}

void dtree_print_helper(DOMTree t, int level, FILE *out_file)
{
    if (dtree_empty(t))
        return;

    indent(level, out_file);
    if (t->tag.type == TAGTYPE_ELEMENT) {
        if (t->tag.num_attrs == 0)
            fprintf(out_file, "<%s>\n", t->tag.content);
        else {
            fprintf(out_file, "<%s", t->tag.content);
            for (int i = 0; i < t->tag.num_attrs; ++i) {
                TagAttr attr = t->tag.attrs[i];
                fprintf(out_file, " %s=\"%s\"", attr_type_string(attr.type),
                        attr.value);
            }
            fprintf(out_file, ">\n");
        }
    } else
        fprintf(out_file, "%s\n", t->tag.content);

    for (int i = 0; i < t->nchild; ++i)
        dtree_print_helper(t->nodes[i], level + 1, out_file);

    if (t->tag.type == TAGTYPE_ELEMENT) {
        indent(level, out_file);
        fprintf(out_file, "</%s>\n", t->tag.content);
    }
}

void indent(int level, FILE *out_file)
{
    for (int i = 0; i < level; ++i)
        fprintf(out_file, "  ");
}

const char *attr_type_string(TagAttrType type)
{
    switch (type) {
        case TAG_ATTR_SRC: return "src";
        case TAG_ATTR_HREF: return "href";
        case TAG_ATTR_ALT: return "alt";
    }

    return "?";
}
