/*
 * A296661  a(n) = (exp(k)*Gamma(1+n, k) - exp(-k)*Gamma(1+n, -k))/k! for k =
 *          3.
 * https://oeis.org/A296661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + 9*a(n-2) - 9*(n-2)*a(n-3), with initial values
 *            a(0)=0, a(1)=1, a(2)=2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Extend the cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 32;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failure – return 0 (unlikely in test environment) */
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing values up to n */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i == 0)
                cache[i] = (seq_t)0;
            else if (i == 1)
                cache[i] = (seq_t)1;
            else if (i == 2)
                cache[i] = (seq_t)2;
            else {
                /* Use seq_t arithmetic; cast idx_t to seq_t where needed */
                seq_t term1 = (seq_t)i * cache[i - 1];
                seq_t term2 = (seq_t)9 * cache[i - 2];
                seq_t term3 = (seq_t)9 * (seq_t)(i - 2) * cache[i - 3];
                cache[i] = term1 + term2 - term3;
            }
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
