/*
 * A356717  a(n) is the integer w such that (c(n)^2, -d(n)^2, w) is a
 *          primitive solution to the Diophantine equation 2*x^3 + 2*y^3 + z^3
 *          = 11^3, where c(n) = F(n+2) + (-1)^n * F(n-3), d(n) = F(n+3) +
 *          (-1)^n * F(n-2) and F(n) is the n-th Fibonacci number (A000045).
 * https://oeis.org/A356717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+7*a(n-2)-7*a(n-3)-a(n-4)+a(n-5) Initial values:
 *            a(1)=1, a(2)=29, a(3)=59, a(4)=241, a(5)=445
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow O(1) retrieval after the first build */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise on first call */
    if (cache_size == 0) {
        cache_size = 5;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return (seq_t)0;           /* allocation failed */
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)29;  /* a(2) */
        cache[2] = (seq_t)59;  /* a(3) */
        cache[3] = (seq_t)241; /* a(4) */
        cache[4] = (seq_t)445; /* a(5) */
    }

    if (n <= 0) return (seq_t)0;              /* not defined for 0 */

    if (n <= cache_size)                      /* already computed */
        return cache[n - 1];

    /* expand cache up to n */
    if (n > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!new_cache) return (seq_t)0;      /* allocation failed */
        cache = new_cache;

        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* using only unsigned arithmetic; the recurrence gives a
               positive result and 'high' is always >= 'low', so
               unsigned subtraction is safe. */
            seq_t high = cache[i - 2] + (seq_t)7 * cache[i - 3] + cache[i - 6];
            seq_t low  = (seq_t)7 * cache[i - 4] + cache[i - 5];
            cache[i - 1] = high - low;
        }
        cache_size = n;
    }
    return cache[n - 1];
}
