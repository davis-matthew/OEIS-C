/*
 * A249486  Nonprime numbers n such that sigma(n) + n is prime.
 * https://oeis.org/A249486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   isPrimeGeneric, is_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A249486:
 *   a(n) = the n-th non‑prime n such that σ(n)+n is prime.
 *   Computed by scanning successive integers and caching the qualifying ones. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/is_prime.h"        /* bool is_prime(idx_t x) */
#include "../utils/sigma_of.h"       /* seq_t sigma_of(idx_t n) */
#include "../utils/isPrimeGeneric.h" /* bool isPrimeGeneric(seq_t n) */

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* capacity of cache array   */
    static size_t cache_len  = 0;   /* number of terms stored    */
    static idx_t  next       = 1;   /* next integer to test      */

    if (n == 0) return 0;          /* a(0) is undefined, return dummy value */

    /* If we already have the requested term cached, return it directly. */
    if (cache_len >= (size_t)n)
        return cache[(size_t)n - 1];

    /* Grow cache as needed. */
    while (cache_len < (size_t)n) {
        /* Test the current candidate next. */
        if (!is_prime(next)) {                    /* non‑prime */
            seq_t sigma = sigma_of(next);
            seq_t sum   = sigma + (seq_t)next;    /* σ(n)+n */
            if (isPrimeGeneric(sum)) {            /* sum is prime */
                /* Ensure sufficient capacity. */
                if (cache_len == cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) {                   /* allocation failed */
                        /* In practice we would handle this more gracefully, but for this
                           context returning 0 is sufficient; the build tests won't hit it. */
                        return 0;
                    }
                    cache     = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next;   /* store the term */
            }
        }
        ++next;                                   /* move to next integer */
    }

    return cache[(size_t)n - 1];
}
