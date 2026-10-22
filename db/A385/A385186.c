/*
 * A385186  Oblong numbers of the form k*(k + 1) such that at least one of k
 *          and k + 1 is prime.
 * https://oeis.org/A385186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) returns the n‑th unique oblong number k*(k+1) such that k
 *            or k+1 is prime (OEIS A385186)
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache      = NULL;   /* array of computed terms      */
    static size_t cache_len = 0;       /* number of terms stored       */
    static size_t cache_cap = 0;       /* allocated capacity           */
    static idx_t   next_k    = 1;      /* next k to evaluate           */

    /* guard against out of bounds access (offset is 1) */
    if (n == 0)
        return 0;

    /* if we already have enough terms, return the cached one */
    if (n <= cache_len)
        return cache[n-1];

    /* expand storage as needed and generate terms until we reach n */
    while (cache_len < n) {
        /* grow the cache when full */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? (cache_cap * 2) : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();              /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* check whether k=next_k or k+1 is prime */
        if (is_prime(next_k) || is_prime(next_k + 1)) {
            seq_t val = (seq_t)next_k * ((seq_t)next_k + 1);
            cache[cache_len++] = val;
        }

        ++next_k;   /* advance to next k */
    }

    return cache[n-1];
}
