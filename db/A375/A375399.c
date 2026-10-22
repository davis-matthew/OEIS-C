/*
 * A375399  Numbers k such that the minima of maximal anti-runs in the weakly
 *          increasing sequence of prime factors of k (with multiplicity) are
 *          not distinct.
 * https://oeis.org/A375399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth number k ≥ 1 such that the minima of the maximal
 *            anti‑runs in the weakly increasing sequence of prime factors of
 *            k (with multiplicity) are not distinct. A k satisfies the
 *            property iff among the minima we find a duplicate
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static seq_t *seq_cache   = NULL;  /* stored values of the sequence */
static size_t cache_cap   = 0;     /* allocated capacity of seq_cache */
static size_t cache_len   = 0;     /* number of values currently cached */
static uint64_t next_candidate = 1;/* smallest integer not yet examined */

/* Determine whether the integer k satisfies the definition of the
   sequence. */
static bool qualifies(uint64_t k)
{
    if (k < 2) return false;          /* 1 has no prime factors */

    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   pcnt  = 0;

    factor_u64(k, primes, exps, &pcnt);

    uint64_t prev   = 0;
    uint64_t minima[128];
    size_t   mcnt   = 0;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        for (uint64_t j = 0; j < e; ++j) {
            if (prev == 0 || p == prev)      /* start of a new anti‑run */
                minima[mcnt++] = p;
            prev = p;
        }
    }

    /* Check for duplicate minima */
    for (size_t i = 0; i < mcnt; ++i) {
        for (size_t j = i + 1; j < mcnt; ++j) {
            if (minima[i] == minima[j]) return true;
        }
    }
    return false;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0;          /* safety for non‑positive n */

    while (cache_len < (size_t)n) {
        /* Advance candidate until a qualifying number is found */
        while (!qualifies(next_candidate)) ++next_candidate;

        /* Grow the cache if necessary */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 16;
            seq_cache = (seq_t*)realloc(seq_cache, cache_cap * sizeof(seq_t));
        }
        seq_cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;                 /* next integer to test */
    }

    return seq_cache[n - 1];
}
