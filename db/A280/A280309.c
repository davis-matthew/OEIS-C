/*
 * A280309  Number of n X 1 0..2 arrays with no element equal to more than one
 *          of its horizontal and vertical neighbors, with the exception of
 *          exactly two elements, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A280309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-2) - 16*a(n-3) + 12*a(n-4) + 24*a(n-5) +
 *            8*a(n-6) Initial values: a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=2,
 *            a(6)=9, a(7)=34, a(8)=124, a(9)=432, a(10)=1464
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static idx_t cache_max = 0;

/* Ensure cache has room for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    /* We need storage for indices 0..n; index 0 is unused */
    size_t needed = (size_t)n + 1;
    if (needed <= cache_capacity) return;

    size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_capacity = new_cap;
}

/* Initialise cache with the first ten terms */
static void init_cache(void)
{
    if (cache != NULL) return;
    ensure_capacity(10);
    for (size_t i = 0; i <= 10; ++i) cache[i] = (seq_t)0;   /* zero out all */
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)0;
    cache[3] = (seq_t)0;
    cache[4] = (seq_t)1;
    cache[5] = (seq_t)2;
    cache[6] = (seq_t)9;
    cache[7] = (seq_t)34;
    cache[8] = (seq_t)124;
    cache[9] = (seq_t)432;
    cache[10] = (seq_t)1464;
    cache_max = 10;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined below offset */
    init_cache();

    if (n > cache_max) {
        ensure_capacity(n);
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i <= 10) {
                /* terms already set during init_cache */
                continue;
            }
            /* recurrence for n > 10 */
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            seq_t a5 = cache[i - 5];
            seq_t a6 = cache[i - 6];

            seq_t val = (seq_t)6 * a1;
            val -= (seq_t)6 * a2;
            val -= (seq_t)16 * a3;
            val += (seq_t)12 * a4;
            val += (seq_t)24 * a5;
            val += (seq_t)8 * a6;

            cache[i] = val;
        }
        cache_max = n;
    }
    return cache[n];
}
