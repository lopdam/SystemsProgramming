#ifndef ARRAY_H
#define ARRAY_H
#include "first.h"

#include "buffer.h"

struct data_unset; /* declaration */

struct data_methods {
	void (*reset)(struct data_unset *p); \
	struct data_unset *(*copy)(const struct data_unset *src); \
	void (*free)(struct data_unset *p); \
	int (*insert_dup)(struct data_unset *dst, struct data_unset *src); \
	void (*print)(const struct data_unset *p, int depth);
};

typedef enum { TYPE_UNSET, TYPE_STRING, TYPE_OTHER, TYPE_ARRAY, TYPE_INTEGER, TYPE_DONOTUSE, TYPE_CONFIG } data_type_t;
#define DATA_UNSET \
	buffer *key; \
	data_type_t type; \
	int is_index_key; /* 1 if key is a array index (autogenerated keys) */ \
	const struct data_methods *fn /* function table */

typedef struct data_unset {
	DATA_UNSET;
} data_unset;

typedef struct {
	data_unset  **data;

	size_t *sorted;

	size_t used; /* <= SSIZE_MAX */
	size_t size;

	size_t unique_ndx;
} array;

typedef struct {
	DATA_UNSET;

	buffer *value;
} data_string;

data_string *data_string_init(void);

typedef struct {
	DATA_UNSET;

	array *value;
} data_array;

data_array *data_array_init(void);

typedef struct {
	DATA_UNSET;

	int value;
} data_integer;

data_integer *data_integer_init(void);

array *array_init(void);
array *array_init_array(array *a);
void array_free(array *a);
void array_reset(array *a);
void array_reset_data_strings(array *a);
void array_insert_unique(array *a, data_unset *entry);
data_unset *array_pop(array *a); /* only works on "simple" lists with autogenerated keys */
int array_is_vlist(array *a);
int array_is_kvany(array *a);
int array_is_kvarray(array *a);
int array_is_kvstring(array *a);
int array_print(array *a, int depth);
#define array_get_element(a, key) array_get_element_klen((a), (key), sizeof(key)-1)
data_unset *array_get_element_klen(const array *a, const char *key, size_t klen);
data_unset *array_extract_element_klen(array *a, const char *key, size_t klen); /* removes found entry from array */
void array_set_key_value(array *hdrs, const char *key, size_t key_len, const char *value, size_t val_len);
void array_insert_key_value(array *hdrs, const char *key, size_t key_len, const char *value, size_t val_len);
void array_insert_value(array *hdrs, const char *value, size_t val_len);
int * array_get_int_ptr(array *a, const char *k, size_t klen);
void array_replace(array *a, data_unset *entry);
void array_print_indent(int depth);
size_t array_get_max_key_length(array *a);

data_unset * array_match_key_prefix_klen (const array * const a, const char * const s, const size_t slen);
data_unset * array_match_key_prefix_nc_klen (const array * const a, const char * const s, const size_t slen);
data_unset * array_match_key_prefix (const array * const a, const buffer * const b);
data_unset * array_match_key_prefix_nc (const array * const a, const buffer * const b);
const buffer * array_match_value_prefix (const array * const a, const buffer * const b);
const buffer * array_match_value_prefix_nc (const array * const a, const buffer * const b);
data_unset * array_match_key_suffix (const array * const a, const buffer * const b);
data_unset * array_match_key_suffix_nc (const array * const a, const buffer * const b);
const buffer * array_match_value_suffix (const array * const a, const buffer * const b);
const buffer * array_match_value_suffix_nc (const array * const a, const buffer * const b);
data_unset * array_match_path_or_ext (const array * const a, const buffer * const b);

#endif
