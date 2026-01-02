#include "tag.h"
#include <stdlib.h>
#include <assert.h>

Tag tag_make(TagType type, char *content)
{
    int num_attrs = 0;
    TagAttr *attrs = calloc(N_ATTRS, sizeof(TagAttr));
    assert(attrs != NULL);
    return (Tag){ type, content, num_attrs, attrs };
}

void tag_free(Tag t)
{
    free(t.attrs);
}

TagAttr tag_attr_make(TagAttrType type, char *value)
{
    return (TagAttr){ type, value };
}

void tag_add_attr(Tag *t, TagAttr attr)
{
    if (t->num_attrs >= N_ATTRS)
        return;

    t->attrs[t->num_attrs++] = attr;
}
