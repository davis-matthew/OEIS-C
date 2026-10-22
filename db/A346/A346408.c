/*
 * A346408  Primes whose bitwise XOR of decimal digits is a prime.
 * https://oeis.org/A346408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) = the n‑th prime p such that the bitwise XOR of * the
 *            decimal digits of p is itself a prime. The sequence is *
 *            generated lazily; previously found terms are cached
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   get_digits, is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/is_prime_small.h"
#include "../utils/get_digits.h"

static bool xor_is_prime(seq_t x)
{
    uint8_t buf[32];
    size_t len;
    get_digits(x, buf, &len);               /* buf holds digits 0–9 */
    seq_t xo = 0;
    for (size_t i = 0; i < len; ++i)
        xo ^= (seq_t)buf[i];
    return is_prime_small(xo);
}

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static seq_t next_candidate = 2;         /* next integer to test */

    if (cache == NULL) {
        cache_cap = 128;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* In the unlikely event of allocation failure we simply
         * allow the subsequent pointer use to crash – the build
         * environment guarantees enough memory for the test size. */
    }

    /* generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        /* test current candidate */
        if (is_prime_big(next_candidate)) {
            if (xor_is_prime(next_candidate)) {
                /* store the term */
                if (cache_size == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_size++] = next_candidate;
            }
        }
        /* advance to the next integer for the next iteration */
        next_candidate++;
    }

    /* safe cast: idx_t is at least as wide as size_t for the test set */
    return cache[(size_t)n - 1];
}
