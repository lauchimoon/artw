#include "tag.h"

Tag tag_make(TagType type, char *content)
{
    return (Tag){type, content};
}
