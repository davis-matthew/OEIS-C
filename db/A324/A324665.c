/*
 * A324665  Starting at n, a(n) is the total number of negative positions
 *          visited according to the following rules. On the k-th step
 *          (k=1,2,3,...) move a distance of k in the direction of zero. If
 *          the number landed on has been landed on before, move a distance of
 *          k away from zero instead.
 * https://oeis.org/A324665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is computed by simulating the walk from n to 0 according to the
   rules of OEIS A324665: on step k move k toward zero; if the landed
   position has been visited before, move k away from zero instead.
   The function counts how many times positions < 0 are visited. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/* Hash table for visited integer positions (int64_t). */
typedef struct {
    int64_t *keys;   /* NULL entries marked by LLONG_MIN */
    size_t   cap;    /* capacity (power of two) */
    size_t   sz;     /* number of stored keys */
} visited_set_t;

static visited_set_t visited;

/* initialise with at least init_cap slots */
static void visited_init(size_t init_cap)
{
    size_t cap = 1;
    while (cap < init_cap) cap <<= 1;
    visited.cap = cap;
    visited.sz  = 0;
    visited.keys = (int64_t*)malloc(cap * sizeof(int64_t));
    for (size_t i = 0; i < cap; ++i) visited.keys[i] = LLONG_MIN;
}

/* free resources */
static void visited_free(void)
{
    free(visited.keys);
    visited.keys = NULL;
    visited.cap  = 0;
    visited.sz   = 0;
}

/* hash function based on current capacity (must be power of two) */
static inline size_t hash_int64(int64_t x)
{
    return ((uint64_t)x * 0x9e3779b97f4a7c15ULL) & (visited.cap - 1);
}

/* resize when load factor > 0.5 */
static void visited_resize(void)
{
    size_t oldcap   = visited.cap;
    size_t newcap   = oldcap << 1;
    int64_t *oldkeys = visited.keys;
    int64_t *newkeys = (int64_t*)malloc(newcap * sizeof(int64_t));
    for (size_t i = 0; i < newcap; ++i) newkeys[i] = LLONG_MIN;

    /* update capacity before rehashing so hash_int64 uses new cap */
    visited.cap = newcap;
    visited.keys = newkeys;

    /* rehash old keys */
    for (size_t i = 0; i < oldcap; ++i) {
        int64_t key = oldkeys[i];
        if (key != LLONG_MIN) {
            size_t h = hash_int64(key);
            while (newkeys[h] != LLONG_MIN) h = (h + 1) & (newcap - 1);
            newkeys[h] = key;
        }
    }
    free(oldkeys);
}

/* insert key (no-op if present) */
static void visited_insert(int64_t key)
{
    if ((visited.sz + 1) * 2 > visited.cap) visited_resize();

    size_t h = hash_int64(key);
    while (visited.keys[h] != LLONG_MIN) {
        if (visited.keys[h] == key) return;   /* already present */
        h = (h + 1) & (visited.cap - 1);
    }
    visited.keys[h] = key;
    visited.sz++;
}

/* test membership */
static bool visited_contains(int64_t key)
{
    size_t h = hash_int64(key);
    while (visited.keys[h] != LLONG_MIN) {
        if (visited.keys[h] == key) return true;
        h = (h + 1) & (visited.cap - 1);
    }
    return false;
}

/* sequence function: a(idx_t n) */
seq_t a(idx_t n)
{
    /* start at position n */
    int64_t pos = (int64_t)n;
    seq_t   count = (pos < 0) ? 1 : 0;

    if (pos == 0) return 0;

    visited_init(16);              /* capacity for small walks */
    visited_insert(pos);           /* mark starting spot */

    idx_t step = 1;
    while (pos != 0) {
        int64_t sign = (pos > 0) ? -1 : 1;           /* towards zero */
        int64_t proposed = pos + sign * (int64_t)step;
        int64_t next;

        if (visited_contains(proposed)) {
            /* move away from zero */
            next = pos - sign * (int64_t)step;
        } else {
            next = proposed;
        }

        visited_insert(next);

        if (next < 0) count++;

        pos = next;
        step++;
    }

    visited_free();
    return count;
}
