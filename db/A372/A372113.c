/*
 * A372113  Numbers k for which (k-1)/2 and 2*k+1 are both primes.
 * https://oeis.org/A372113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 887, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9690 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = k where (k-1)/2 and 2*k+1 are both prime.
   Computed by a linear search using prime tests.  */
/* Code implements seq_t a(idx_t n)  */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;          /* number of terms stored in cache  */
    static idx_t  next_k = 5;         /* next odd k to test                */

    /* initialize cache once */
    if (!cache) {
        cap   = 16;
        cache = mem_new(seq_t, cap);
    }

    /* generate terms until we reach the requested index */
    while (len < n) {
        /* test conditions for current_k = next_k */
        idx_t half = (next_k - 1) / 2;
        idx_t two_k_plus1 = 2 * next_k + 1;

        if (is_prime(half) && is_prime(two_k_plus1)) {
            /* store the found term */
            if (len == cap) {               /* grow cache if needed */
                cap *= 2;
                cache = mem_resize(seq_t, cache, cap);
            }
            cache[len++] = (seq_t)next_k;
        }

        next_k += 2;   /* only odd candidates need checking */
    }

    /* return the requested term (1‑based indexing) */
    return cache[n - 1];
}
