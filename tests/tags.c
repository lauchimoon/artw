#include "../tag.h"
#include "../dtree.h"
#include <stdio.h>

int main()
{
    Tag html_tag = tag_make(TAGTYPE_ELEMENT, "html");
    Tag body_tag = tag_make(TAGTYPE_ELEMENT, "body");
    Tag img_tag = tag_make(TAGTYPE_ELEMENT, "img");
    tag_add_attr(&img_tag, tag_attr_make(TAG_ATTR_SRC, "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcR9iNG8n8qtW0xy6qpdD9ia3jFjBcI_zksHdKw3USZRocebqAHnESOzia4&s=10"));
    tag_add_attr(&img_tag, tag_attr_make(TAG_ATTR_ALT, "Beautiful cat"));

    DOMTree root = dtree_make();
    root = dtree_insert(root, html_tag);
    dtree_insert(root, body_tag);
    DOMTree body = root->nodes[root->nchild - 1];
    dtree_insert(body, img_tag);

    tag_free(img_tag);
    tag_free(body_tag);
    tag_free(html_tag);
    dtree_print(root, stdout);
    dtree_free(root);
    return 0;
}
