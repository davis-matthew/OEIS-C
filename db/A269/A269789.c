/*
 * A269789  Primes p such that 2*p + 59 is a square.
 * https://oeis.org/A269789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form 2*k^2 + 2*k - 29 (equivalently
 *            the n-th prime p such that 2*p+59 is a square). The function
 *            returns a(idx_t n) with 1‑based indexing
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* cache of found primes */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* smallest k that may produce a prime; the first prime occurs at k=4 */
    static idx_t next_k = 4;

    if (n == 0) {
        /* offset is 1, but for safety return 0 for n==0 */
        return 0;
    }

    /* grow cache capacity if necessary */
    if (cache_len < n) {
        if (cache_cap == 0) {
            cache_cap = 16;
        }
        while (cache_cap < n) {
            cache_cap <<= 1;
        }
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* generate primes until we have enough */
    while (cache_len < n) {
        seq_t k = (seq_t)next_k;
        /* p = 2*k^2 + 2*k - 29 */
        seq_t p = 2 * k * k + 2 * k - 29;
        if (p >= 2 && is_prime((idx_t)p)) {
            cache[cache_len++] = p;
        }
        ++next_k;
    }

    return cache[n - 1];
}
