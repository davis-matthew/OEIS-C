/*
 * A324661  Starting at n, a(n) is the total number of moves made to the left
 *          according to the following rules. On the k-th step (k=1,2,3,...)
 *          move a distance of k in the direction of zero. If the number
 *          landed on has been landed on before, move a distance of k away
 *          from zero instead.
 * https://oeis.org/A324661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   ht_hash
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simulate the walk defined in A324661 and count the moves
   that go to the left (decreasing integers).  The algorithm
   stores all visited positions in a small open‑addressing
   hash table; it stops when the position reaches 0,
   which is guaranteed to happen for every n. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ht_hash.h"

/* -- Simple open‑addressing hash table for int64_t keys ---------- */
typedef struct {
    size_t cap;        /* capacity (power of two)                       */
    size_t sz;         /* number of stored keys                          */
    int64_t *keys;     /* array of keys                                 */
    unsigned char *used; /* bitmap: 0=empty, 1=occupied              */
} HashTable;

static void ht_init(HashTable *h, size_t cap)
{
    /* Round up to a power of two, at least 8. */
    if (cap < 8) cap = 8;
    while ((cap & (cap - 1)) != 0) cap <<= 1;
    h->cap   = cap;
    h->sz    = 0;
    h->keys  = (int64_t *)malloc(cap * sizeof(int64_t));
    h->used  = (unsigned char *)calloc(cap, sizeof(unsigned char));
}

static void ht_free(HashTable *h)
{
    free(h->keys);
    free(h->used);
}

static bool ht_contains(const HashTable *h, int64_t key)
{
    size_t idx = ht_hash(key, h->cap);
    while (h->used[idx]) {
        if (h->keys[idx] == key) return true;
        idx = (idx + 1) & (h->cap - 1);
    }
    return false;
}

static void ht_insert(HashTable *h, int64_t key)
{
    /* Rehash if the load factor exceeds 0.7. */
    if ((h->sz + 1) * 10 > h->cap * 7) {
        /* Double the capacity. */
        size_t newcap = h->cap << 1;
        HashTable tmp;
        ht_init(&tmp, newcap);
        for (size_t i = 0; i < h->cap; ++i) {
            if (h->used[i]) ht_insert(&tmp, h->keys[i]);
        }
        free(h->keys);
        free(h->used);
        h->keys = tmp.keys;
        h->used = tmp.used;
        h->cap  = tmp.cap;
        h->sz   = tmp.sz;
    }

    size_t idx = ht_hash(key, h->cap);
    while (h->used[idx]) {
        if (h->keys[idx] == key) return; /* already present */
        idx = (idx + 1) & (h->cap - 1);
    }
    h->used[idx] = 1;
    h->keys[idx] = key;
    ++h->sz;
}

/* ------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Special case: already at zero. */
    if (n == 0) return 0;

    HashTable visited;
    ht_init(&visited, 8);
    ht_insert(&visited, (int64_t)n);

    seq_t left_moves = 0;
    int64_t pos = (int64_t)n;

    for (idx_t k = 1; ; ++k) {
        if (pos == 0) break;

        int sgn = (pos > 0) ? 1 : -1;
        int64_t tentative = pos - (int64_t)sgn * (int64_t)k;
        int64_t new_pos;

        if (!ht_contains(&visited, tentative))
            new_pos = tentative;                 /* move toward zero */
        else
            new_pos = pos + (int64_t)sgn * (int64_t)k; /* bounce away */

        if (new_pos < pos)            /* leftward move (decreasing value) */
            ++left_moves;

        ht_insert(&visited, new_pos);
        pos = new_pos;
    }

    ht_free(&visited);
    return left_moves;
}
