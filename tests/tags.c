#include "../tag.h"
#include <stdio.h>

void print_tag(Tag t);

int main()
{
    printf("img tag\n");
    Tag t = tag_make(TAGTYPE_ELEMENT, "img");
    tag_add_attr(&t, tag_attr_make(TAG_ATTR_SRC, "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcR9iNG8n8qtW0xy6qpdD9ia3jFjBcI_zksHdKw3USZRocebqAHnESOzia4&s=10"));
    tag_add_attr(&t, tag_attr_make(TAG_ATTR_ALT, "Beautiful cat"));
    print_tag(t);
    tag_free(t);

    printf("\na tag\n");
    t = tag_make(TAGTYPE_ELEMENT, "a");
    tag_add_attr(&t, tag_attr_make(TAG_ATTR_HREF, "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcR9iNG8n8qtW0xy6qpdD9ia3jFjBcI_zksHdKw3USZRocebqAHnESOzia4&s=10"));
    print_tag(t);
    tag_free(t);

    printf("\np tag\n");
    t = tag_make(TAGTYPE_ELEMENT, "p");
    print_tag(t);
    tag_free(t);

    return 0;
}

void print_tag(Tag t)
{
    if (t.num_attrs < 1) {
        printf("no attr\n");
        return;
    }

    for (int i = 0; i < t.num_attrs; ++i) {
        TagAttr attr = t.attrs[i];
        switch (attr.type) {
            case TAG_ATTR_SRC: printf("src: "); break;
            case TAG_ATTR_HREF: printf("href: "); break;
            case TAG_ATTR_ALT: printf("alt: "); break;
        }

        if (attr.value)
            printf("%s\n", attr.value);
    }
}
