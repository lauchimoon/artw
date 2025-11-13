#ifndef DTREE_H
#define DTREE_H

#include "tag.h"

struct DOMTag {
    Tag tag;
    int nchild;
    int maxchild;
    struct DOMTag **nodes;
};

typedef struct DOMTag *DOMTree;

DOMTree dtree_make(void);
int dtree_empty(DOMTree t);
DOMTree dtree_insert(DOMTree t, Tag tag);
void dtree_print(DOMTree t);

#endif // DTREE_H
