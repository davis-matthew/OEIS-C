/*
 * A244607  Numbers k such that (product of digits of k) - 1 is prime.
 * https://oeis.org/A244607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = k such that (product_of_digits(k) - 1) is prime. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_product.h"
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t last_checked = 0;

    /* Increase the cache until it contains at least n terms. */
    while (cache_len < n)
    {
        /* Increments candidate value, starting from 1. */
        last_checked++;
        idx_t k = last_checked;

        /* Compute the product of the decimal digits of k. */
        seq_t prod = digit_product(k);

        /* prod must be at least 2 to produce a positive number after subtracting 1. */
        if (prod <= 1)
            continue;

        seq_t pminus1 = prod - 1;

        /* Check primality of prod-1. */
        if (is_prime_big(pminus1))
        {
            /* Ensure the cache has room for one more element. */
            ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = k;
        }
    }

    /* The cache is now guaranteed to hold at least n terms; return the nth. */
    return cache[n - 1];
}
