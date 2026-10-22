/*
 * A360309  a(n) = Sum_{k=0..floor(n/3)} binomial(n-1-2*k,n-3*k) *
 *          binomial(2*k,k).
 * https://oeis.org/A360309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(n-1-2k,n-3k) * binomial(2k,k)
 *            Recurrence: for n ≥ 4 n*a(n) = 2*(n-1)*a(n-1) - (n-2)*a(n-2) +
 *            2*(2*n-3)*a(n-3) - 2*(2*n-6)*a(n-4) Initial values: a(0)=1,
 *            a(1)=0, a(2)=0, a(3)=2 The function memoises computed values so
 *            calls in any order are fast
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of elements stored */

    /* Initialise cache with first four terms */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(4 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)2;
        cache_len = 4;
    }

    /* Grow cache if necessary */
    if ((size_t)(n + 1) > cache_len) {
        cache = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failed */

        for (idx_t i = cache_len; i <= n; ++i) {
            /* i ≥ 4 here because cache_len ≥ 4 initially */
            seq_t term1 = 2 * (seq_t)(i - 1) * cache[i - 1];
            seq_t term2 = (seq_t)(i - 2) * cache[i - 2];
            seq_t term3 = 2 * (seq_t)(2 * i - 3) * cache[i - 3];
            seq_t term4 = 2 * (seq_t)(2 * i - 6) * cache[i - 4];
            seq_t rhs   = term1 - term2 + term3 - term4;
            cache[i]    = rhs / (seq_t)i;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
