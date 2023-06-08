#include "Clip.h"
#include <cstdlib>
#include <cstdarg>

Clip::Clip()
{
    size = 1000;
    data = (LexType*)malloc(1000 * sizeof(LexType));
    cur = data;
}

Clip::Clip(int size)
{
    size = size;
    data = (LexType*)malloc(size * sizeof(LexType));
    cur = data;
}


LexType Clip::get()
{
    LexType value = *cur;
    cur--;
    return value;
}


int Clip::set(LexType value)
{
    if (cur + 1 - data > size) {
        return 1;
    }
    cur++;
    *cur = value;
    return 0;
}

int Clip::set_array(int count, ...)
{
    va_list args;
    va_start(args, count);
    LexType* lexes = new LexType[count];

    for (int i = 0; i < count; i++) {
        lexes[i] = va_arg(args, LexType);
    }

    for (int i = 0; i < count; i++) {
        int set_result = set(lexes[count - i - 1]);
        if (set_result)
            return 1;
    }

    return 0;
}
