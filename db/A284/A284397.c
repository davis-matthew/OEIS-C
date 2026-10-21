/*
 * A284397  a(1) = 1, a(2) = 2; a(n) is the largest prime <= (a(a(n-1)) +
 *          a(n-a(n-2)-1)) for n > 2.
 * https://oeis.org/A284397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 791, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A284397
   a(1) = 1, a(2) = 2; a(n) = largest prime ≤ ( a(a(n-1)) + a(n - a(n-2) - 1) ), n>2
   This implementation memoises the values in a static cache to allow
   calls in any order.  The helper is_prime_big from the utils directory
   is used to find the largest prime ≤ x. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Return the largest prime ≤ x (x ≥ 2).  Uses is_prime_big to test
   primality.  Works for all supported seq_t types. */
static seq_t largest_prime_leq(seq_t x)
{
    while (!is_prime_big(x))
        x -= (seq_t)1;
    return x;
}

seq_t a(idx_t n)
{
    /* a(1) = 1, a(2) = 2, cache[i] = a(i+1) for i ≥ 0 */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of values stored */
    static size_t cache_cap = 0;   /* capacity of cache */

    if (n < 1)   /* undefined for n < 1, but guard anyway */
        return (seq_t)0;

    /* Grow cache up to requested index n (1‑based) */
    while (cache_len < n) {
        idx_t m = cache_len + 1;   /* current term index we are computing */
        seq_t value;

        if (m == 1)
            value = (seq_t)1;
        else if (m == 2)
            value = (seq_t)2;
        else {
            /* a(m-1) is stored at cache[m-2] */
            idx_t idx_a_m_minus_1 = (idx_t)cache[m - 2];
            seq_t term1 = cache[idx_a_m_minus_1 - 1];

            /* a(m-2) is stored at cache[m-3] */
            idx_t idx_a_m_minus_2 = (idx_t)cache[m - 3];
            idx_t second_idx = (idx_t)(m - idx_a_m_minus_2 - 1);

            seq_t term2 = cache[second_idx - 1];

            seq_t sum = term1 + term2;
            value = largest_prime_leq(sum);
        }

        /* Allocate or grow cache if needed */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = value;
    }

    return cache[n - 1];
}
