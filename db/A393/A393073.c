/*
 * A393073  Convolution of the two tribonacci sequences A000073 and A001590.
 * https://oeis.org/A393073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 3*a(n-4) - 2*a(n-5) - a(n-6); initial
 *            terms: a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=1, a(5)=3 This
 *            recurrence holds for n ≥ 6. The function memoises results so
 *            that any order of calls returns the correct value
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 36, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* memoised terms      */
static size_t cache_cap = 0;      /* allocated capacity  */
static idx_t  cache_len = 0;      /* number of terms stored */

seq_t a(idx_t n)
{
    /* Ensure enough space for requested index. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first six terms on first call. */
    if (cache_len == 0) {
        cache_len = 6;
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 1;
        cache[4] = 1;
        cache[5] = 3;
    }

    /* Compute and store terms up to the required index. */
    while (cache_len <= (size_t)n) {
        idx_t k = cache_len;           /* index of new term     */
        seq_t val =
            (2 * cache[k-1] + cache[k-2])          /* 2*a(k-1) + a(k-2)   */
            - (3 * cache[k-4] + 2 * cache[k-5] + cache[k-6]); /* 3*a(k-4)+2*a(k-5)+a(k-6) */
        cache[cache_len++] = val;
    }

    return cache[n];
}
