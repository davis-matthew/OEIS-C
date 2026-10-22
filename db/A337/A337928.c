/*
 * A337928  Numbers w such that (F(2n+1)^2, -F(2n)^2, -w) are primitive
 *          solutions of the Diophantine equation 2*x^3 + 2*y^3 + z^3 = 1,
 *          where F(n) is the n-th Fibonacci number (A000045).
 * https://oeis.org/A337928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=5, a(2)=31; a(n) = 8*a(n-1) - 8*a(n-2) + a(n-3) for
 *            n>2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    /* Allocate or grow the cache if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t newcap = cache_cap ? cache_cap : 8;
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = newcap;
    }
    /* Initialize first values */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }
    if (cache_len < 2 && n >= 1) {
        cache[1] = 5;
        cache_len = 2;
    }
    if (cache_len < 3 && n >= 2) {
        cache[2] = 31;
        cache_len = 3;
    }
    /* Compute values up to n using the recurrence */
    for (size_t i = (size_t)cache_len; i <= (size_t)n; ++i) {
        seq_t v1 = cache[i-1];
        seq_t v2 = cache[i-2];
        seq_t v3 = cache[i-3];
        cache[i] = v1*8 - v2*8 + v3;
    }
    return cache[(size_t)n];
}
