/*
 * A290896  p-INVERT of the positive integers, where p(S) = 1 - S^8.
 * https://oeis.org/A290896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - 120*a(n-2) + 560*a(n-3) - 1820*a(n-4) +
 *            4368*a(n-5) - 8008*a(n-6) + 11440*a(n-7) - 12869*a(n-8) +
 *            11440*a(n-9) - 8008*a(n-10) + 4368*a(n-11) - 1820*a(n-12) +
 *            560*a(n-13) - 120*a(n-14) + 16*a(n-15) - a(n-16) a(0) = 0, a(1)
 *            = 0, a(2) = 0, a(3) = 0, a(4) = 0, a(5) = 0, a(6) = 0, a...
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised terms */
static idx_t  cache_size = 0;    /* number of terms stored, last index = cache_size-1 */
static size_t cache_cap = 0;     /* allocated capacity */

/* Ensure capacity for at least "need" elements */
static void ensure_capacity(idx_t need)
{
    if ((size_t)cache_cap >= need) return;
    if (cache_cap == 0) cache_cap = 16;
    while ((size_t)cache_cap < need) cache_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    if (tmp == NULL) abort();          /* out of memory */
    cache = tmp;
}

seq_t a(idx_t n)
{
    /* Initialise memoisation table once */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        /* first 16 values of the sequence */
        cache[0]  = 0;
        cache[1]  = 0;
        cache[2]  = 0;
        cache[3]  = 0;
        cache[4]  = 0;
        cache[5]  = 0;
        cache[6]  = 0;
        cache[7]  = 1;
        cache[8]  = 16;
        cache[9]  = 136;
        cache[10] = 816;
        cache[11] = 3876;
        cache[12] = 15504;
        cache[13] = 54264;
        cache[14] = 170544;
        cache[15] = 490315;
        cache_size = 16;
    }

    if (n < cache_size) return cache[n];

    /* Compute missing values up to n */
    ensure_capacity(n + 1); /* need indices 0 .. n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val =
            (seq_t)16   * cache[i-1] -
            (seq_t)120  * cache[i-2] +
            (seq_t)560  * cache[i-3] -
            (seq_t)1820 * cache[i-4] +
            (seq_t)4368 * cache[i-5] -
            (seq_t)8008 * cache[i-6] +
            (seq_t)11440* cache[i-7] -
            (seq_t)12869* cache[i-8] +
            (seq_t)11440* cache[i-9] -
            (seq_t)8008 * cache[i-10] +
            (seq_t)4368 * cache[i-11] -
            (seq_t)1820 * cache[i-12] +
            (seq_t)560  * cache[i-13] -
            (seq_t)120  * cache[i-14] +
            (seq_t)16   * cache[i-15] -
            cache[i-16];
        cache[i] = val;
    }
    cache_size = n + 1;
    return cache[n];
}
