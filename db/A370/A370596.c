/*
 * A370596  Numbers k such that A007814(k) is a prime number.
 * https://oeis.org/A370596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = all numbers whose binary representation has a prime
 *            number of trailing zeros. This is equivalent to numbers of the
 *            form 2^p · (2k+1) where p is prime and k ≥ 0
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below generates the sequence lazily, caching all
   computed terms between calls.  It scans successive integers,
   keeping the last examined value, and keeps those with a prime
   count of trailing zeros. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached sequence values   */
    static idx_t  cache_len  = 0;   /* number of cached terms   */
    static size_t cache_cap  = 0;   /* allocated array capacity */
    static seq_t  last       = 3;   /* last integer examined    */

    /* Produce further terms until we have at least n of them. */
    while (cache_len < n) {
        /* Grow the cache if needed. */
        if (cache_cap == 0) {
            cache_cap = 8;
            cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        } else if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
        }

        /* Check the next integer. */
        last += 1;
        seq_t tmp = last;
        size_t tz = 0;               /* trailing zero count */
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            tz++;
        }

        /* Keep the number if its trailing‑zero count is prime. */
        if (is_prime((idx_t)tz))
            cache[cache_len++] = last;
    }

    return cache[n - 1];
}
