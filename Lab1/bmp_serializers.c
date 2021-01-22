#include <stdio.h>
#include <malloc.h>
#include "bmp_struct.h"
#include "bmp_serializers.h"

enum read_status from_bmp( FILE* in, struct image* img ) {
    if (!in) { return READ_INVALID_FILE_ADDRESS; }

    struct bmp_header header = {};

    fread(&header, 1, sizeof (struct bmp_header), in);
    if (header.bfType != 19778) { return READ_INVALID_SIGNATURE; }
    if (header.biBitCount != 24) { return READ_INVALID_BITS; }
    if (header.biSize != 40
        || header.biCompression != 0
        || header.bfileSize != header.bOffBits + header.biSizeImage) {
        return READ_INVALID_HEADER;
    }

    uint8_t* file_data = (uint8_t*) malloc(header.biSizeImage);
    fseek(in, header.bOffBits, SEEK_SET);
    fread(file_data, 1, header.biSizeImage, in);
    uint64_t data_size = header.biHeight * header.biWidth * sizeof(struct pixel);
    img -> data = (struct pixel*) malloc(data_size);
    img -> height = header.biHeight;
    img -> width = header.biWidth;
    uint16_t padding = header.biWidth % 4;

    for (size_t i = 0; i < header.biHeight; i = i + 1) {
        for (size_t j = 0; j < header.biWidth; j = j + 1) {
            size_t pixel_ij = i * img -> width + j;
            img -> data[pixel_ij] = *(struct pixel*) ((file_data) + sizeof(struct pixel) * (pixel_ij) + padding * i);
        }
    }

    fclose(in);
    return READ_OK;
}

enum write_status to_bmp( FILE* out, struct image const* img ) {
    struct bmp_header header = {
            .bfType = 19778,
            .bfileSize = img -> width * img -> height * sizeof(struct pixel) + img -> height * (img -> width % 4) +
                    sizeof(struct bmp_header),
            .bfReserved = 0,
            .bOffBits = sizeof(struct bmp_header),
            .biSize = 40,
            .biWidth = img -> width,
            .biHeight = img -> height,
            .biPlanes = 1,
            .biBitCount = 24,
            .biCompression = 0,
            .biSizeImage = header.bfileSize - header.bOffBits,
            .biXPelsPerMeter = 0,
            .biYPelsPerMeter = 0,
            .biClrUsed = 0,
            .biClrImportant = 0
    };

    uint64_t data_size = (img -> width + ((img -> width) % 4)) * (img -> height) * sizeof(struct pixel);
    uint8_t *data = malloc(data_size);
    for (size_t i = 0; i < img -> height; i = i + 1) {
        for (size_t j = 0; j < img -> width; j = j + 1) {
            size_t pixel_ij = i * (img -> width) + j;
            *((struct pixel *) (data + sizeof(struct pixel) * pixel_ij + i * ((img -> width) % 4))) = img -> data[pixel_ij];
        }
    }

    if(!fwrite(&header, 1, sizeof(struct bmp_header), out) ||
            !fwrite(data, 1, (img -> width + ((img -> width) % 4)) * img -> height * sizeof(struct pixel), out)){
        return WRITE_ERROR;
    }

    return WRITE_OK;
}

enum read_status open_bmp(FILE* in) {

}