#include <malloc.h>
#include "rotate.h"

/* создаёт копию изображения, которая повёрнута на 90 градусов */
struct image rotate( struct image const source ) {
    struct image rotated = { .height = source.width, .width = source.height,
            malloc(source.height * source.width * sizeof(struct pixel)) };

    for (size_t i = 0; i < source.height; i = i + 1) {
        for (size_t j = 0; j < source.width; j = j + 1) {
            rotated.data[i + ((rotated.height - j -1) * rotated.width)] = source.data[j + i * source.width];
        }
    }

    return rotated;
}
