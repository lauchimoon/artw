#ifndef DTREE_H
#define DTREE_H

#include "tag.h"
#include <stdio.h>

struct DOMTag {
    Tag tag;
    int nchild;
    int maxchild;
    struct DOMTag **nodes;
};

typedef struct DOMTag *DOMTree;

DOMTree dtree_make(void);
void dtree_free(DOMTree t);
int dtree_empty(DOMTree t);
DOMTree dtree_insert(DOMTree t, Tag tag);
void dtree_print(DOMTree t, FILE *out_file);

#endif // DTREE_H
