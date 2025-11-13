#ifndef TAG_H
#define TAG_H

typedef enum {
    TAGTYPE_ELEMENT = 0,  // <...>
    TAGTYPE_TEXT,         // ...
} TagType;

typedef struct Tag {
    TagType type;
    char *content;
} Tag;

Tag tag_make(TagType type, char *content);

#endif // TAG_H
