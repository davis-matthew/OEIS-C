/*
 * A320030  Automaton sum similar to A102376 but using mod 3 instead of mod 2.
 * https://oeis.org/A320030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by iterating a 2‑D cellular automaton over Z/3Z
 *            with rule c(i,j) = (c(i+1,j-1)+c(i+1,j+1)+c(i-1,j-1)+c(i-1,j+1))
 *            mod 3, starting with a single cell of value 1 at generation 1.
 *            The function returns the sum of all cell values at generation n
 * Range:     u8 n <= 17, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   hash_int64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/hash_int64.h"

/* ---------------------------------------------------------------------- */

/* encode coordinates to a 64‑bit key (high 32 bits = i, low 32 bits = j) */
static inline int64_t encode(int64_t i, int64_t j)
{
    return (i << 32) | (int64_t)(uint32_t)j;
}

#define EMPTY_KEY ((int64_t)1 << 63) /* never occurs as (i<<32)|j */

/* Naïve hash table for sparse cellular automaton states */
typedef struct {
    size_t          capacity;    /* power of two */
    size_t          mask;        /* capacity-1 */
    int64_t        *keys;        /* encoded coordinates */
    int             *vals;        /* cell values (0,1,2) */
    size_t         *used_idx;    /* indices of occupied slots */
    size_t          used_cnt;    /* number of active cells */
} HashMap;

/* initialise hash map with at least 'need' entries */
static void hashmap_init(HashMap *h, size_t need)
{
    size_t sz = 1;
    while (sz < need * 2) sz <<= 1;          /* > 2 * need */
    h->capacity = sz;
    h->mask     = sz - 1;
    h->keys     = (int64_t *)malloc(sz * sizeof(int64_t));
    h->vals     = (int *)malloc(sz * sizeof(int));
    h->used_idx = (size_t *)malloc(sz * sizeof(size_t));
    for (size_t i = 0; i < sz; ++i) h->keys[i] = EMPTY_KEY;
    h->used_cnt = 0;
}

/* reset hash map for reuse: clear all keys */
static void hashmap_reset(HashMap *h)
{
    for (size_t i = 0; i < h->used_cnt; ++i)
        h->keys[h->used_idx[i]] = EMPTY_KEY;
    h->used_cnt = 0;
}

/* add a value to a key (insert if missing) */
static void hashmap_add(HashMap *h, int64_t k, int add)
{
    size_t idx = hash_int64(k) & h->mask;
    while (1) {
        int64_t existing = h->keys[idx];
        if (existing == EMPTY_KEY) {          /* free slot found */
            h->keys[idx] = k;
            h->vals[idx] = add;
            h->used_idx[h->used_cnt++] = idx;
            return;
        }
        if (existing == k) {                  /* same key */
            h->vals[idx] += add;
            return;
        }
        idx = (idx + 1) & h->mask;            /* linear probe */
    }
}

/* ---------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* n == 0 is unused in OEIS offset, return 0 */
    if (n == 0) return 0;
    if (n == 1) return 1;   /* initial cell 1 */

    /* initialise two hash maps for the current and next generations */
    const size_t INIT_CAP = 1 << 16;   /* 65536 slots */
    HashMap cur, nxt;
    hashmap_init(&cur, INIT_CAP);
    hashmap_init(&nxt, INIT_CAP);

    /* first generation: single cell (0,0) with value 1 */
    hashmap_add(&cur, encode(0, 0), 1);

    /* evolve generations 2 .. n */
    for (idx_t gen = 2; gen <= n; ++gen) {
        hashmap_reset(&nxt);   /* clear next generation */

        /* for every active cell in current generation */
        for (size_t i = 0; i < cur.used_cnt; ++i) {
            size_t pos = cur.used_idx[i];
            int64_t key = cur.keys[pos];
            int     v   = cur.vals[pos];
            int64_t i_coord = key >> 32;
            int64_t j_coord = (int32_t)key;  /* low 32 bits signed */

            /* add to four diagonal neighbours */
            hashmap_add(&nxt, encode(i_coord + 1, j_coord - 1), v);
            hashmap_add(&nxt, encode(i_coord + 1, j_coord + 1), v);
            hashmap_add(&nxt, encode(i_coord - 1, j_coord - 1), v);
            hashmap_add(&nxt, encode(i_coord - 1, j_coord + 1), v);
        }

        /* reduce all neighbour sums modulo 3 and discard zeros */
        size_t new_used = 0;
        for (size_t i = 0; i < nxt.used_cnt; ++i) {
            size_t pos = nxt.used_idx[i];
            int r = nxt.vals[pos] % 3;
            if (r == 0) {
                nxt.keys[pos] = EMPTY_KEY;        /* mark empty */
            } else {
                nxt.vals[pos] = r;
                nxt.used_idx[new_used++] = pos;
            }
        }
        nxt.used_cnt = new_used;

        /* swap current and next for the following generation */
        HashMap tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum all cell values of the last generation */
    seq_t sum = 0;
    for (size_t i = 0; i < cur.used_cnt; ++i) {
        size_t pos = cur.used_idx[i];
        sum += (seq_t)cur.vals[pos];
    }

    return sum;
}
