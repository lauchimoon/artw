#include "../tag.h"
#include <stdio.h>

void print_attr(TagAttr attr);

int main()
{
    printf("img tag\n");
    Tag t = tag_make(TAGTYPE_ELEMENT, "img");
    tag_add_attr(&t, tag_attr_make(TAG_ATTR_SRC, "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcR9iNG8n8qtW0xy6qpdD9ia3jFjBcI_zksHdKw3USZRocebqAHnESOzia4&s=10"));
    tag_add_attr(&t, tag_attr_make(TAG_ATTR_ALT, "Beautiful cat"));

    for (int i = 0; i < t.num_attrs; ++i)
        print_attr(t.attrs[i]);

    tag_free(t);

    printf("\na tag\n");
    t = tag_make(TAGTYPE_ELEMENT, "a");
    tag_add_attr(&t, tag_attr_make(TAG_ATTR_HREF, "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcR9iNG8n8qtW0xy6qpdD9ia3jFjBcI_zksHdKw3USZRocebqAHnESOzia4&s=10"));
    print_attr(t.attrs[0]);
    tag_free(t);

    return 0;
}

void print_attr(TagAttr attr)
{
    switch (attr.type) {
        case TAG_ATTR_SRC: printf("src: "); break;
        case TAG_ATTR_HREF: printf("href: "); break;
        case TAG_ATTR_ALT: printf("alt: "); break;
    }

    printf("%s\n", attr.value);
}
