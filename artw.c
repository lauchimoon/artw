#include <stdio.h>
#include "dtree.h"

int main()
{
    DOMTree root = dtree_make();
    Tag html_tag = tag_make(TAGTYPE_ELEMENT, "html");
    Tag body_tag = tag_make(TAGTYPE_ELEMENT, "body");
    Tag p_tag = tag_make(TAGTYPE_ELEMENT, "p");
    Tag text = tag_make(TAGTYPE_TEXT, "Hola mundo");

    root = dtree_insert(root, html_tag);

    DOMTree html = root;
    dtree_insert(html, body_tag);

    DOMTree body = html->nodes[html->nchild - 1];
    dtree_insert(body, p_tag);

    DOMTree p = body->nodes[body->nchild - 1];
    dtree_insert(p, text);

    dtree_print(root);
    return 0;
}
