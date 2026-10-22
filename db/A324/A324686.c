/*
 * A324686  Starting at n, a(n) is the number of times we move away from zero
 *          from a negative position according to the following rules. On the
 *          k-th step (k=1,2,3,...) move a distance of k in the direction of
 *          zero. If the number landed on has been landed on before, move a
 *          distance of k away.
 * https://oeis.org/A324686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of times the walk starting at n moves away from
 *            zero while starting from a negative position according to the
 *            rule described
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Simple open‑addressing hash set for int64_t keys. */
typedef struct {
    int64_t key;
    unsigned char used;
} HashEntry;

typedef struct {
    HashEntry *entries;
    size_t cap;   /* power of two */
    size_t size;  /* number of used entries */
} Int64HashSet;

/* Knuth's multiplicative hash for int64_t. */
static inline uint64_t hash_int64(int64_t x)
{
    uint64_t y = (uint64_t)x;
    y ^= y >> 33;
    y *= 0xff51afd7ed558ccdULL;
    y ^= y >> 33;
    y *= 0xc4ceb9fe1a85ec53ULL;
    y ^= y >> 33;
    return y;
}

/* Create a new hash set with the given capacity (must be power of two). */
static HashEntry *hash_set_create(size_t *cap)
{
    *cap = 16;
    HashEntry *arr = (HashEntry *)calloc(*cap, sizeof(HashEntry));
    return arr;
}

/* Free the hash set. */
static void hash_set_free(HashEntry *arr, size_t cap)
{
    free(arr);
}

/* Resize the hash set to twice the current capacity. */
static void hash_set_resize(Int64HashSet *s)
{
    size_t new_cap = s->cap << 1;
    HashEntry *new_arr = (HashEntry *)calloc(new_cap, sizeof(HashEntry));
    size_t mask = new_cap - 1;
    for (size_t i = 0; i < s->cap; ++i) {
        if (s->entries[i].used) {
            int64_t key = s->entries[i].key;
            uint64_t h = hash_int64(key);
            for (size_t j = 0; j < new_cap; ++j) {
                size_t idx = (h + j) & mask;
                if (!new_arr[idx].used) {
                    new_arr[idx].used = 1;
                    new_arr[idx].key = key;
                    break;
                }
            }
        }
    }
    free(s->entries);
    s->entries = new_arr;
    s->cap = new_cap;
}

/* Insert key into the hash set. If key already present, nothing is changed. */
static void hash_set_insert(Int64HashSet *s, int64_t key)
{
    if (((double)(s->size + 1) / (double)s->cap) > 0.5)
        hash_set_resize(s);

    uint64_t h = hash_int64(key);
    size_t mask = s->cap - 1;
    for (size_t i = 0; i < s->cap; ++i) {
        size_t idx = (h + i) & mask;
        if (!s->entries[idx].used) {
            s->entries[idx].used = 1;
            s->entries[idx].key = key;
            s->size++;
            return;
        }
        if (s->entries[idx].key == key)
            return;
    }
}

/* Return non-zero if key is present; otherwise zero. */
static int hash_set_contains(const Int64HashSet *s, int64_t key)
{
    uint64_t h = hash_int64(key);
    size_t mask = s->cap - 1;
    for (size_t i = 0; i < s->cap; ++i) {
        size_t idx = (h + i) & mask;
        if (!s->entries[idx].used)
            return 0;
        if (s->entries[idx].key == key)
            return 1;
    }
    return 0;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    /* Use signed 64‑bit for the walk positions.  The initial position never
       exceeds 2^63−1 for the test data (n ≤ 10 000).  All arithmetic fits
       safely in int64_t. */
    int64_t pos = (int64_t)n;

    /* Hash set for visited positions. */
    Int64HashSet visited;
    visited.entries = hash_set_create(&visited.cap);
    visited.size = 0;
    visited.cap = visited.cap;  /* already set by hash_set_create */

    hash_set_insert(&visited, pos);

    seq_t count = 0;

    for (idx_t k = 1; ; ++k) {
        if (pos == 0)
            break;

        int sign = (pos > 0) ? 1 : -1;          /* direction towards zero */
        int64_t target;
        if (sign > 0)
            target = pos - (int64_t)k;          /* towards zero */
        else
            target = pos + (int64_t)k;          /* towards zero */

        if (hash_set_contains(&visited, target)) {
            /* Target already visited – move away from zero. */
            int64_t newpos;
            if (sign > 0)
                newpos = pos + (int64_t)k;      /* away */
            else
                newpos = pos - (int64_t)k;      /* away */

            if (pos < 0)  /* starting from a negative position */
                count++;

            pos = newpos;
            hash_set_insert(&visited, pos);
        } else {
            /* Move towards zero. */
            pos = target;
            hash_set_insert(&visited, pos);
        }
    }

    hash_set_free(visited.entries, visited.cap);
    return (seq_t)count;
}
