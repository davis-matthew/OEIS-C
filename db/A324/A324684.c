/*
 * A324684  Starting at n, a(n) is the number of times we move from a positive
 *          spot to a spot we have already visited according to the following
 *          rules. On the k-th step (k=1,2,3,...) move a distance of k in the
 *          direction of zero. If the number landed on has been landed on
 *          before, move a distance of k away.
 * https://oeis.org/A324684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the number of times the simulated walk (from the
   OEIS entry A324684) moves from a positive spot to an already visited
   spot. The walk starts at n and, for k = 1, 2, …, it attempts to move
   a distance k towards zero; if that tentative spot has been visited
   before, it instead moves a distance k away from zero. The walk stops
   when it reaches zero.  The returned value is the count of the
   described moves. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple hash set for int64_t keys, for the visited positions. */
typedef struct {
    int64_t *keys;
    unsigned char *used;   /* 0 = empty, 1 = occupied */
    size_t   cap;
    size_t   size;
} int64_set;

/* Simple 64‑bit hash function. */
static inline size_t hash_int64(int64_t key)
{
    uint64_t h = (uint64_t)key;
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccdULL;
    h ^= h >> 33;
    h *= 0xc4ceb9fe1a85ec53ULL;
    h ^= h >> 33;
    return (size_t)h;
}

/* Initialise the set with a power‑of‑two capacity. */
static void set_init(int64_set *s, size_t init_cap)
{
    size_t cap = 1;
    while (cap < init_cap) cap <<= 1;
    s->keys = (int64_t *)malloc(cap * sizeof(int64_t));
    s->used = (unsigned char *)calloc(cap, sizeof(unsigned char));
    s->cap  = cap;
    s->size = 0;
}

/* Rehash the set to a larger capacity. */
static void set_expand(int64_set *s)
{
    size_t new_cap = s->cap << 1;
    int64_t *new_keys = (int64_t *)malloc(new_cap * sizeof(int64_t));
    unsigned char *new_used = (unsigned char *)calloc(new_cap, sizeof(unsigned char));

    for (size_t i = 0; i < s->cap; ++i) {
        if (s->used[i]) {
            int64_t key = s->keys[i];
            size_t pos = hash_int64(key) & (new_cap - 1);
            while (new_used[pos]) pos = (pos + 1) & (new_cap - 1);
            new_keys[pos] = key;
            new_used[pos] = 1;
        }
    }
    free(s->keys);
    free(s->used);
    s->keys = new_keys;
    s->used = new_used;
    s->cap  = new_cap;
}

/* Insert a key if it is not already present. */
static void set_insert(int64_set *s, int64_t key)
{
    if (s->size >= s->cap * 3 / 4) set_expand(s);
    size_t pos = hash_int64(key) & (s->cap - 1);
    while (s->used[pos]) {
        if (s->keys[pos] == key) return; /* already present */
        pos = (pos + 1) & (s->cap - 1);
    }
    s->keys[pos] = key;
    s->used[pos] = 1;
    ++s->size;
}

/* Test whether a key is present. */
static bool set_contains(const int64_set *s, int64_t key)
{
    size_t pos = hash_int64(key) & (s->cap - 1);
    while (s->used[pos]) {
        if (s->keys[pos] == key) return true;
        pos = (pos + 1) & (s->cap - 1);
    }
    return false;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    int64_set vis;
    set_init(&vis, 64);
    int64_t cur = (int64_t)n;
    set_insert(&vis, cur);

    seq_t count = 0;

    for (idx_t k = 1; cur != 0; ++k) {
        int64_t dir = (cur > 0) ? -1 : 1;                 /* toward zero */
        int64_t tentative = cur + dir * (int64_t)k;        /* intended spot */

        bool seen_tentative = set_contains(&vis, tentative);
        int64_t new_pos = seen_tentative ? cur - dir * (int64_t)k
                                         : tentative;      /* apply rule */

        bool seen_new = set_contains(&vis, new_pos);
        if (cur > 0 && seen_new) ++count;                 /* positive→already visited */

        set_insert(&vis, new_pos);
        cur = new_pos;
    }

    free(vis.keys);
    free(vis.used);
    return count;
}
