/*
 * A390731  Numbers k not in A000961 such that A001414(k) is in A000961.
 * https://oeis.org/A390731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer k such that * k is NOT a prime power,
 *            and the sum of its * prime factors (counted with multiplicity)
 *            is a prime power. * Computation: Sequentially test integers for
 *            the above property * and cache the successful ones, so that any
 *            a(n) can be returned * in O(1) after the se...
 * Range:     u8 n <= 71, u16 n <= 9722, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Utility for prime factorisation with exponents. */
#include "../utils/factor_u64.h"

/* Cache for the generated sequence. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t next_candidate = 2;

/* Increase the cache reservoir (used only by the generator). */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return the n-th element of sequence A390731 (a(1) is the first). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined but safe */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        uint64_t k;
        /* Find next qualifying integer. */
        while (1) {
            k = next_candidate++;

            /* Factor k. */
            uint64_t primes[64];
            uint64_t exps  [64];
            size_t   cnt   = 0;
            factor_u64(k, primes, exps, &cnt);

            if (cnt == 1) continue;            /* k is a prime power → skip */

            /* Compute the sum of prime factors (with multiplicity). */
            uint64_t sum = 0;
            for (size_t i = 0; i < cnt; ++i)
                sum += primes[i] * exps[i];

            /* Check that this sum itself is a prime power. */
            factor_u64(sum, primes, exps, &cnt);
            if (cnt == 1) break;               /* found a term */
        }

        /* Store the found term. */
        if (cache_len >= cache_cap) grow_cache();
        cache[cache_len++] = (seq_t)k;
    }

    return cache[(size_t)n - 1];
}
