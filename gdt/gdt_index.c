#include <stdlib.h>
#include <string.h>

#include "gdt_index.h"

#define STRING_SECTION_INIT_SIZE 256

void
char_buffer_init(struct char_buffer *b, size_t sz)
{
    b->data = malloc(sz);
    b->data[0] = 0;
    b->length = 0;
    b->size = sz;
}

void
char_buffer_free(struct char_buffer *b)
{
    free(b->data);
}

static void
char_buffer_resize(struct char_buffer *b, size_t req_size)
{
    size_t curr_size = b->size;
    while (req_size > curr_size)
    {
        curr_size *= 2;
    }
    char *new_data = malloc(curr_size);
    memcpy(new_data, b->data, b->length + 1);
    free(b->data);
    b->data = new_data;
    b->size = curr_size;
}

int
char_buffer_append(struct char_buffer *b, const char *str)
{
    size_t len = strlen(str);
    int string_offset = (b->length > 0 ? b->length + 1 : 0);
    size_t new_len = string_offset + len;
    if (new_len + 1 > b->size)
        char_buffer_resize(b, new_len + 1);
    memcpy(b->data + string_offset, str, len + 1);
    b->length = new_len;
    return string_offset;
}

gdt_index *
gdt_index_new(int alloc_size)
{
    size_t extra_size = sizeof(int) * (alloc_size - INDEX_AUTO);
    gdt_index *g = malloc(sizeof(gdt_index) + extra_size);
    char_buffer_init(g->names, STRING_SECTION_INIT_SIZE);
    g->length = 0;
    g->size = alloc_size;
    return g;
}

void
gdt_index_free(gdt_index *g)
{
    char_buffer_free(g->names);
    free(g);
}

gdt_index *
gdt_index_resize(gdt_index *g)
{
    size_t alloc_size = g->size * 2;
    size_t extra_size = sizeof(int) * (alloc_size - INDEX_AUTO);
    gdt_index *new_g = malloc(sizeof(gdt_index) + extra_size);

    new_g->names[0] = g->names[0];
    new_g->length = g->length;
    new_g->size = alloc_size;
    memcpy(new_g->index, g->index, sizeof(int) * g->length);

    free(g);
    return new_g;
}

int
gdt_index_add(gdt_index *g, const char *str)
{
    /* if the index table is not big enough return error code */
    if (g->length + 1 > g->size)
        return (-1);

    /* add the given string in the key string buffer */
    int str_offset = char_buffer_append(g->names, str);

    /* add the new string offset index into the index table */
    int idx = g->length;
    g->index[idx] = str_offset;
    g->length ++;
    return idx;
}

const char *
gdt_index_get(gdt_index *g, int index)
{
    if (index < 0 || index >= g->length)
        return NULL;
    return g->names->data + g->index[index];
}

int
gdt_index_lookup(gdt_index *g, const char *req)
{
    const char *base = g->names->data;
    for (int k = 0; k < g->length; k++)
    {
        const char *str = base + g->index[k];
        if (strcmp(str, req) == 0)
            return k;
    }
    return (-1);
}
