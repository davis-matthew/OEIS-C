/*
 * A324673  Starting at n, a(n) is the length of the smallest interval
 *          containing all points visited according to the following rules. On
 *          the k-th step (k=1,2,3,...) move a distance of k in the direction
 *          of zero. If the number landed on has been landed on before, move a
 *          distance of k away.
 * https://oeis.org/A324673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A324673 – walk toward zero with bounce.
   a(n) = length of the interval containing all visited points
   starting at position n until the walk first lands on zero. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Simple open‑addressing hash set for signed 64‑bit integers. */
typedef struct {
    int64_t *data;          /* keys */
    unsigned char *used;    /* occupancy flag */
    size_t cap;             /* capacity (power of two) */
    size_t sz;              /* number of occupied slots */
} VisSetLL;

/* FNV‑1a 64‑bit hash for int64_t values. */
static size_t hash_int64(int64_t key)
{
    uint64_t h = 14695981039346656037ULL;
    uint64_t k = (uint64_t)key;
    for (int i = 0; i < 8; ++i) {
        uint8_t byte = (uint8_t)k;
        h ^= byte;
        h *= 1099511628211ULL;
        k >>= 8;
    }
    return (size_t)h;
}

static void vsll_init(VisSetLL *vs)
{
    vs->cap = 16;
    vs->sz  = 0;
    vs->data = (int64_t *)calloc(vs->cap, sizeof(int64_t));
    vs->used = (unsigned char *)calloc(vs->cap, sizeof(unsigned char));
}

static void vsll_free(VisSetLL *vs)
{
    free(vs->data);
    free(vs->used);
}

static void vsll_resize(VisSetLL *vs)
{
    size_t new_cap = vs->cap << 1;
    int64_t *new_data = (int64_t *)calloc(new_cap, sizeof(int64_t));
    unsigned char *new_used = (unsigned char *)calloc(new_cap, sizeof(unsigned char));

    for (size_t i = 0; i < vs->cap; ++i) {
        if (!vs->used[i]) continue;
        size_t h = hash_int64(vs->data[i]) & (new_cap - 1);
        while (new_used[h]) h = (h + 1) & (new_cap - 1);
        new_data[h] = vs->data[i];
        new_used[h] = 1;
    }

    free(vs->data);
    free(vs->used);
    vs->data = new_data;
    vs->used = new_used;
    vs->cap = new_cap;
}

static bool vsll_contains(const VisSetLL *vs, int64_t key)
{
    if (vs->sz == 0) return false;
    size_t h = hash_int64(key) & (vs->cap - 1);
    while (vs->used[h]) {
        if (vs->data[h] == key) return true;
        h = (h + 1) & (vs->cap - 1);
    }
    return false;
}

static void vsll_insert(VisSetLL *vs, int64_t key)
{
    if ((vs->sz + 1) * 5 >= vs->cap * 4) vsll_resize(vs);
    size_t h = hash_int64(key) & (vs->cap - 1);
    while (vs->used[h]) h = (h + 1) & (vs->cap - 1);
    vs->data[h] = key;
    vs->used[h] = 1;
    ++vs->sz;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Use 64‑bit signed integers for positions – enough for n ≤ 10 000. */
    int64_t cur = (int64_t)n;
    int64_t mn   = cur;
    int64_t mx   = cur;

    VisSetLL vs;
    vsll_init(&vs);
    vsll_insert(&vs, cur);

    for (idx_t step = 1;; ++step) {
        int64_t d   = (int64_t)step;
        int64_t dir = (cur > 0) ? -1 : 1;   /* direction toward zero */
        int64_t target = cur + dir * d;     /* first try */

        if (vsll_contains(&vs, target)) {   /* already visited → go away */
            target = cur - dir * d;
        }

        vsll_insert(&vs, target);

        if (target < mn) mn = target;
        if (target > mx) mx = target;
        cur = target;

        if (cur == 0) break;
    }

    vsll_free(&vs);
    return (seq_t)(mx - mn);
}
