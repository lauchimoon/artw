#ifndef TAG_H
#define TAG_H

#define N_ATTRS 2

typedef enum {
    TAGTYPE_ELEMENT = 0,  // <...>
    TAGTYPE_TEXT,         // ...
} TagType;

typedef enum {
    TAG_ATTR_SRC = 0,
    TAG_ATTR_HREF,
    TAG_ATTR_ALT,
    TAG_ATTR_REL,
} TagAttrType;

typedef struct TagAttr {
    TagAttrType type;
    char *value;
} TagAttr;

typedef struct TagAttrArray {
    TagAttr *items;
    int len, cap;
} TagAttrArray;

typedef struct Tag {
    TagType type;
    char *content;
    int num_attrs;
    TagAttr *attrs; // TODO: make it dynamic
} Tag;

Tag tag_make(TagType type, char *content);
void tag_free(Tag t);
TagAttr tag_attr_make(TagAttrType type, char *value);
void tag_add_attr(Tag *t, TagAttr attr);

#endif // TAG_H
