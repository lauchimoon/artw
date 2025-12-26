#include <stdio.h>
#include "dtree.h"

int main()
{
    DOMTree root = dtree_make();
    Tag html_tag = tag_make(TAGTYPE_ELEMENT, "html");
    Tag body_tag = tag_make(TAGTYPE_ELEMENT, "body");
    Tag p_tag = tag_make(TAGTYPE_ELEMENT, "p");
    Tag p_tag2 = tag_make(TAGTYPE_ELEMENT, "p");
    Tag text = tag_make(TAGTYPE_TEXT, "Hello world!");
    Tag text2 = tag_make(TAGTYPE_TEXT, "Goodbye world!");

    root = dtree_insert(root, html_tag);

    DOMTree html = root;
    dtree_insert(html, body_tag);

    DOMTree body = html->nodes[html->nchild - 1];
    dtree_insert(body, p_tag);
    dtree_insert(body, p_tag2);

    DOMTree p = body->nodes[body->nchild - 2];
    dtree_insert(p, text);

    DOMTree p2 = body->nodes[body->nchild - 1];
    dtree_insert(p2, text2);

    dtree_print(root);
    dtree_free(root);
    return 0;
}
