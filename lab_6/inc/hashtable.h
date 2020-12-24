#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

extern int max_collisions;

#define CACHE_HIT               0
#define CACHE_MISS              -1

typedef enum hashtype_e
{
    SIMPLE,
    COMPLICATED
} hashtype_t;

#define SIMPLE_INSERT(ht, val)       hash_insert(ht, val, SIMPLE)
#define COMPLICATED_INSERT(ht, val)  hash_insert(ht, val, COMPLICATED)

typedef struct hashelem_s
{
    int val;
    int free;
} hashelem_t;

typedef struct hashtable_s
{
    hashelem_t *arr;
    unsigned int len;
} hashtable_t;

unsigned int next_prime(unsigned int num);

int hash_simple(int val, int len);
int hash_complicated(int val, int len);

hashtable_t *hash_init(unsigned int len);
void hash_clean(hashtable_t **ht);

int hash_insert(hashtable_t *ht, const int val, const hashtype_t type);
int hash_find(const hashtable_t *ht, const int key, const hashtype_t type);
int hash_remove(hashtable_t *ht, const int key, const hashtype_t type);

hashtable_t *hash_resize(hashtable_t *ht, unsigned int len);

void hash_print(const hashtable_t *ht);

#endif // __HASHTABLE_H__
