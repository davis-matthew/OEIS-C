/*
 * A244885  Expansion of (1-6*x+12*x^2-8*x^3+x^4)/((1-2*x)^2*(1-3*x+x^2)).
 * https://oeis.org/A244885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A244885
   Formula (used for calculation):
        a(0) = 1
        a(n) = Fibonacci(2*n+1) - (n+1) * 2^(n-2)   for n > 0
   Equivalent linear recurrence (used for fast computation):
        a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=14
        a(n) = 7*a(n-1) - 17*a(n-2) + 16*a(n-3) - 4*a(n-4)   for n > 4
   This implementation memoises computed terms up to the maximum index
   requested, so that calls can be in arbitrary order.  C and C++ builds
   are compatible (the cast performs the necessary conversion from void*
   to seq_t* for both languages).
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* initialise cache with first 5 terms on first call */
    if (cache == NULL) {
        cache_cap = 5;
        cache_len  = 5;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)14;
    }

    /* return already memoised value */
    if (n < cache_len)
        return cache[n];

    /* extend cache up to the requested index */
    while (cache_len <= n) {
        idx_t i = cache_len;

        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];

        /* compute next value using the recurrence */
        seq_t val = (seq_t)7 * a1 - (seq_t)17 * a2 + (seq_t)16 * a3 - (seq_t)4 * a4;

        /* grow cache if necessary */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_len] = val;
        cache_len++;
    }

    return cache[n];
}
