#include "dtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHILD_MIN 256

DOMTree make_node(Tag tag);
void tagcpy(Tag *dst, Tag src);
void dtree_print_helper(DOMTree t, int level);
void indent(int level);

DOMTree dtree_make(void)
{
    return NULL;
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
            for (int i = old_cap; i < t->maxchild; ++i)
                t->nodes[i] = NULL;
        }
    }

    t->nodes[t->nchild++] = node;
    return t;
}

void dtree_print(DOMTree t)
{
    dtree_print_helper(t, 0);
}

DOMTree make_node(Tag tag)
{
    DOMTree node = malloc(sizeof(struct DOMTag));
    tagcpy(&node->tag, tag);
    node->nchild = 0;
    node->maxchild = MAX_CHILD_MIN;
    node->nodes = malloc(node->maxchild*sizeof(struct DOMTag));
    for (int i = 0; i < node->maxchild; ++i)
        node->nodes[i] = dtree_make();

    return node;
}

void tagcpy(Tag *dst, Tag src)
{
    dst->type = src.type;
    dst->content = strdup(src.content);
}

void dtree_print_helper(DOMTree t, int level)
{
    if (dtree_empty(t))
        return;

    indent(level);
    if (t->tag.type == TAGTYPE_ELEMENT)
        printf("<%s>\n", t->tag.content);
    else
        printf("%s\n", t->tag.content);

    for (int i = 0; i < t->nchild; ++i)
        dtree_print_helper(t->nodes[i], level + 1);

    if (t->tag.type == TAGTYPE_ELEMENT) {
        indent(level);
        printf("</%s>\n", t->tag.content);
    }
}

void indent(int level)
{
    for (int i = 0; i < level; ++i)
        printf("  ");
}
