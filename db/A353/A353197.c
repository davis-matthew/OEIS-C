/*
 * A353197  Numbers k such that k + s + k*s is prime, where s is the sum of
 *          digits of k.
 * https://oeis.org/A353197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer k such that k + s + k*s is prime, where
 *            s = digit_sum(k)
 * Range:     u8 n <= 2, u16 n <= 1780, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;
    static idx_t next_k = 1;

    /* Ensure cache has enough capacity */
    if (cache == NULL)
    {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n)
    {
        seq_t k = (seq_t)next_k;
        seq_t s = digit_sum(k);
        seq_t p = k + s + k * s;

        if (isPrimeGeneric(p))
        {
            if (cache_len == cache_cap)
            {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = k;
        }
        next_k++;
    }

    return cache[n - 1];
}
