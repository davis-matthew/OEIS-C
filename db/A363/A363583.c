/*
 * A363583  Numbers k such that 2*phi(k)+k is a prime, where phi is A000010.
 * https://oeis.org/A363583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   calc_phi, is_prime_big, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 2*phi(k)+k is prime (OEIS A363583)
   Computation: iterate over odd square‑free k and test primality of
   2*phi(k)+k. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/calc_phi.h"
#include "../utils/is_prime_big.h"

/* Static cache for the sequence values.  The cache is indexed from 0
   but the sequence is 1‑based: a(1) is the first term. */
static seq_t *cache = NULL;
static size_t cache_len   = 0; /* number of terms stored               */
static size_t cache_cap   = 0; /* capacity of the cache array          */
static seq_t   next_k     = 1; /* next odd candidate to test           */

seq_t a(idx_t n)
{
    if (n == 0)          /* behaviour for indices below the offset is irrelevant */
        return 0;

    /* Generate terms until we have at least n values in the cache. */
    while (cache_len < n) {
        seq_t k = next_k;

        while (cache_len < n) {
            /* Skip even numbers altogether. */
            if (!is_squarefree(k)) {
                k += 2;
                continue;
            }

            seq_t phi = calc_phi(k);
            seq_t val = 2 * phi + k;

            if (is_prime_big(val)) {
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    /* realloc handles both C and C++ */
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = k;
            }
            k += 2;               /* advance to the next odd candidate */
        }
        next_k = k;               /* remember where we left off */
    }

    return cache[n - 1];
}
