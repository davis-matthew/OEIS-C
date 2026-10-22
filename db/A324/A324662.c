/*
 * A324662  Starting at n, a(n) is the difference of the number of left moves
 *          and the number of right moves according to the following rules. On
 *          the k-th step (k=1,2,3,...) move a distance of k in the direction
 *          of zero. If the number landed on has been landed on before, move a
 *          distance of k away from zero in...
 * https://oeis.org/A324662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = difference of left moves and right moves in the process
 *            described in OEIS A324662
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple open-addressing hash set for signed 64-bit integers. */
typedef struct {
    size_t cap;         /* capacity (power of two) */
    size_t len;         /* number of stored keys */
    int64_t *keys;      /* keys array */
    uint8_t *used;      /* 0 = empty, 1 = occupied */
} iset_t;

/* Murmur-inspired 64‑bit hash for int64_t keys. */
static inline size_t hash_int64(int64_t x)
{
    uint64_t h = (uint64_t)x;
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccdULL;
    h ^= h >> 33;
    h *= 0xc4ceb9fe1a85ec53ULL;
    h ^= h >> 33;
    return (size_t)h;
}

static void iset_init(iset_t *s, size_t init_cap)
{
    size_t cap = 1;
    while (cap < init_cap) cap <<= 1;
    s->cap = cap;
    s->len = 0;
    s->keys = (int64_t*)calloc(cap, sizeof(int64_t));
    s->used = (uint8_t*)calloc(cap, sizeof(uint8_t));
}

static void iset_free(iset_t *s)
{
    free(s->keys);
    free(s->used);
    s->cap = s->len = 0;
}

static void iset_resize(iset_t *s)
{
    size_t old_cap = s->cap;
    int64_t *old_keys = s->keys;
    uint8_t *old_used = s->used;

    s->cap <<= 1;
    s->keys = (int64_t*)calloc(s->cap, sizeof(int64_t));
    s->used = (uint8_t*)calloc(s->cap, sizeof(uint8_t));
    s->len = 0;

    size_t mask = s->cap - 1;
    for (size_t i = 0; i < old_cap; ++i) {
        if (old_used[i]) {
            int64_t key = old_keys[i];
            size_t idx = hash_int64(key) & mask;
            while (s->used[idx]) idx = (idx + 1) & mask;
            s->used[idx] = 1;
            s->keys[idx] = key;
            s->len++;
        }
    }
    free(old_keys);
    free(old_used);
}

static bool iset_contains(iset_t *s, int64_t key)
{
    size_t mask = s->cap - 1;
    size_t idx = hash_int64(key) & mask;
    while (s->used[idx]) {
        if (s->keys[idx] == key) return true;
        idx = (idx + 1) & mask;
    }
    return false;
}

static void iset_insert(iset_t *s, int64_t key)
{
    if ((double)s->len / s->cap > 0.65) iset_resize(s);

    size_t mask = s->cap - 1;
    size_t idx = hash_int64(key) & mask;
    while (s->used[idx]) {
        if (s->keys[idx] == key) return;   /* already present */
        idx = (idx + 1) & mask;
    }
    s->used[idx] = 1;
    s->keys[idx] = key;
    s->len++;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    int64_t current = (int64_t)n;
    int left = 0, right = 0;

    iset_t visited;
    iset_init(&visited, 16);
    iset_insert(&visited, current);

    idx_t k = 1;           /* step number */

    while (current != 0) {
        int sign_toward = (current > 0) ? -1 : 1;   /* -1: left, +1: right */
        int64_t target = current + (int64_t)sign_toward * (int64_t)k;

        int64_t change;
        if (iset_contains(&visited, target)) {
            change = -(int64_t)sign_toward * (int64_t)k;
        } else {
            change = (int64_t)sign_toward * (int64_t)k;
        }
        current += change;

        if (change < 0) ++left; else ++right;
        iset_insert(&visited, current);

        ++k;
    }

    iset_free(&visited);
    return (seq_t)(left - right);
}
