/*
 * A321985  Numbers m such that m^2+1 is semiprime with (m-1)^2+1 and
 *          (m+1)^2+1 primes.
 * https://oeis.org/A321985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 5, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n) by searching for m such that
   (m-1)^2+1 and (m+1)^2+1 are prime and (m^2+1)/2 is prime,
   i.e. m^2+1 = 2·r with r prime.  The sequence consists of
   all such odd integers m, in ascending order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined offset, but safe */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_m = 3;        /* first odd candidate */

    while (cache_len < (size_t)n) {
        const seq_t m      = next_m;
        const seq_t m_minus = m - 1;
        const seq_t m_plus  = m + 1;
        const seq_t p = m_minus * m_minus + 1;
        const seq_t q = m_plus  * m_plus  + 1;
        const seq_t r = (m * m + 1) / 2;

        if (is_prime_big(p) && is_prime_big(q) && is_prime_big(r)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {               /* allocation failure */
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = m;
        }

        next_m += 2;        /* only odd m need be considered */
    }

    return cache[n - 1];
}
