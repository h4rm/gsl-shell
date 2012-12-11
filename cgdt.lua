ffi = require 'ffi'

ffi.cdef [[
struct char_buffer {
    char *data;
    size_t length;
    size_t size;
};

typedef struct {
    struct char_buffer names[1];
    int length;
    int size;
    int index[4];
} gdt_index;

extern gdt_index *   gdt_index_new         (int alloc_size);
extern void          gdt_index_free        (gdt_index *g);
extern gdt_index *   gdt_index_resize      (gdt_index *g);
extern int           gdt_index_add         (gdt_index *g, const char *str);
extern const char *  gdt_index_get         (gdt_index *g, int index);
extern int           gdt_index_lookup      (gdt_index *g, const char *req);

typedef struct {
    int tag;
    double number;
} gdt_element;

typedef struct {
    int size;
    gdt_element *data;
    int ref_count;
} gdt_block;

typedef struct {
    int size1;
    int size2;
    int tda;
    gdt_element *data;
    gdt_block *block;
    gdt_index *strings;
    gdt_index *headers;
} gdt_table;

extern gdt_table *         gdt_table_new                (int nb_rows, int nb_columns, int nb_rows_alloc);
extern void                gdt_table_free               (gdt_table *t);
extern const gdt_element * gdt_table_get                (gdt_table *t, int i, int j);
extern const char *        gdt_table_element_get_string (gdt_table *t, const gdt_element *e);
extern void                gdt_table_set_number         (gdt_table *t, int i, int j, double num);
extern void                gdt_table_set_string         (gdt_table *t, int i, int j, const char *s);
extern const char *        gdt_table_get_header         (gdt_table *t, int j);
extern void                gdt_table_set_header         (gdt_table *t, int j, const char *str);

]]

return ffi.load("libgdt")