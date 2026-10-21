/*
 * A247024  Primes of the form x*(2^x - x) - 1.
 * https://oeis.org/A247024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big, mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute primes of the form x*(2^x - x) - 1 by iterating over x
   until the required number of primes are found.   */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_x = 2;       /* next x to test   */
    static seq_t power2 = 0;       /* 2^next_x         */

    /* one‑time initialization */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        power2 = pow2(2);    /* 2^2 = 4 for x = 2 */
        next_x = 2;
    }

    /* generate primes until we have at least n of them */
    while (cache_len < n) {
        /* p = x * (2^x - x) - 1  */
        seq_t t = power2 - (seq_t)next_x;           /* 2^x - x   */
        seq_t p = mul_small(t, (unsigned int)next_x);/* x * (2^x - x) */
        p = p - 1;                                   /* minus 1   */

        if (is_prime_big(p)) {
            if (cache_len == cache_cap) {   /* grow cache */
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = p;
        }

        /* advance to next x */
        next_x++;
        power2 = mul_small(power2, 2); /* 2^(x+1) = 2^x * 2 */
    }

    return cache[n - 1];
}
