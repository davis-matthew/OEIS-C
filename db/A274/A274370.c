/*
 * A274370  Let the starting square of Langton's ant have coordinates (0, 0),
 *          with the ant looking in negative x-direction. a(n) is the
 *          y-coordinate of the ant after n moves.
 * https://oeis.org/A274370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, easy
 * Formula:   a(0..10080) are the y‑coordinates of Langton's ant after each
 *            move, a(n) = a(n-1) + a(n-104) - a(n-105) for n >= 10081,
 *            a(n+104) = a(n) - 2 for n > 9975 (recurrence follows the OEIS
 *            definition)
 * Range:     i8 n <= 9854, i16 n <= 9854, i32 n <= 9854, i64 n <= 9854,
 *            i128 n <= 9854, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define HASH_BITS 16
#define HASH_SIZE (1U << HASH_BITS)

static int32_t  ht_x[HASH_SIZE];
static int32_t  ht_y[HASH_SIZE];
static uint8_t  ht_val[HASH_SIZE];
static unsigned char ht_used[HASH_SIZE] = {0};

static inline size_t ht_index(int32_t x, int32_t y)
{
    uint32_t ux = (uint32_t)x;
    uint32_t uy = (uint32_t)y;
    return ((size_t)(ux * 2654435761u) ^ (size_t)(uy * 1597334677u)) & (HASH_SIZE - 1);
}

static uint8_t ht_get(int32_t x, int32_t y)
{
    size_t idx = ht_index(x, y);
    while (ht_used[idx]) {
        if (ht_x[idx] == x && ht_y[idx] == y) return ht_val[idx];
        idx = (idx + 1) & (HASH_SIZE - 1);
    }
    return 0;               /* default white */
}

static void ht_put(int32_t x, int32_t y, uint8_t val)
{
    size_t idx = ht_index(x, y);
    while (ht_used[idx]) {
        if (ht_x[idx] == x && ht_y[idx] == y) {
            ht_val[idx] = val;
            return;
        }
        idx = (idx + 1) & (HASH_SIZE - 1);
    }
    ht_x[idx]   = x;
    ht_y[idx]   = y;
    ht_val[idx] = val;
    ht_used[idx] = 1;
}

/* Cache for sequence values */
static seq_t *cache    = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void cache_ensure(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < need) new_cap <<= 1;
#ifdef __cplusplus
    seq_t *newp = new seq_t[new_cap]();   /* zero‑initialize */
    for (size_t i = 0; i < cache_len; ++i) {
        ::new(&newp[i]) seq_t(cache[i]);   /* copy construct */
    }
    delete[] cache;
    cache = newp;
#else
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp) exit(1);
    cache = newp;
#endif
    cache_cap = new_cap;
}

/* Ant simulation state */
static int32_t ant_x = 0;
static int32_t ant_y = 0;
static unsigned char ant_dir = 3;   /* 0N,1E,2S,3W  (start West) */

seq_t a(idx_t n)
{
    /* lazy initialization */
    static int initialized = 0;
    if (!initialized) {
        cache_ensure(1);
        cache[0] = (seq_t)0;
        cache_len = 1;
        ant_x = 0; ant_y = 0; ant_dir = 3;
        memset(ht_used, 0, sizeof(ht_used));
        initialized = 1;
    }

    size_t target = (size_t)n;
    if (cache_len <= target) {
        cache_ensure(target + 1);
        for (size_t i = cache_len; i <= target; ++i) {
            if (i <= 10080) {          /* simulation step */
                uint8_t color = ht_get(ant_x, ant_y);
                if (color == 0) {      /* white -> turn right (clockwise) */
                    ant_dir = (ant_dir + 1) & 3;
                    ht_put(ant_x, ant_y, 1);   /* paint black */
                } else {               /* black -> turn left (counter‑clockwise) */
                    ant_dir = (ant_dir + 3) & 3;
                    ht_put(ant_x, ant_y, 0);   /* paint white */
                }
                /* move forward */
                switch (ant_dir) {
                    case 0: ++ant_y; break; /* North  */
                    case 1: ++ant_x; break; /* East   */
                    case 2: --ant_y; break; /* South  */
                    case 3: --ant_x; break; /* West   */
                }
                cache[i] = (seq_t)ant_y;    /* y‑coordinate after i moves */
            } else {                       /* recurrence for large n */
                cache[i] = cache[i-1] + cache[i-104] - cache[i-105];
            }
        }
        cache_len = target + 1;
    }
    return cache[target];
}
