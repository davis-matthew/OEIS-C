/*
 * A265063  Coordination sequence for (2,4,8) tiling of hyperbolic plane.
 * https://oeis.org/A265063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 59, u64 n <= 122,
 *            u128 n <= 247, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+a(n-3)-a(n-4)+a(n-5)+a(n-7)-a(n-8) for n≥9
 * with initial values:
 * a(0)=1, a(1)=3, a(2)=5, a(3)=8, a(4)=12, a(5)=17, a(6)=25,
 * a(7)=37, a(8)=53.  The recurrence follows from the rational
 * generating function in the OEIS entry.
 */
#include <stddef.h>
#include <stdlib.h>

/* static cache for memoised values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* ensure the cache contains all values up to n */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initialise the first 9 terms */
        cache_cap = 9;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)12;
        cache[5] = (seq_t)17;
        cache[6] = (seq_t)25;
        cache[7] = (seq_t)37;
        cache[8] = (seq_t)53;
        cache_len = 9;
    }

    if (cache_len > (size_t)n)
        return;          /* the requested value is already cached */

    /* enlarge cache so that it can hold indices up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t a_n_1 = cache[i - 1];
        seq_t a_n_3 = cache[i - 3];
        seq_t a_n_4 = cache[i - 4];
        seq_t a_n_5 = cache[i - 5];
        seq_t a_n_7 = cache[i - 7];
        seq_t a_n_8 = cache[i - 8];

        seq_t val = a_n_1 + a_n_3 + a_n_5 + a_n_7;
        seq_t sub = a_n_4 + a_n_8;
        cache[i] = val - sub;   /* subtraction is safe for unsigned type */
    }
    cache_len = (size_t)n + 1;
}

/* main sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
