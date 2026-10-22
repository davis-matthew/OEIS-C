/*
 * A369954  Numbers k that are neither squarefree nor prime powers and also
 *          coprime to 6.
 * https://oeis.org/A369954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1856, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A369954: Numbers k that are coprime to 6,
   not squarefree, not prime powers, and with either (k–1) or (k+1)
   also having the same property.  The function builds the
   sequence on demand, caching results so repeated calls are fast. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* ----------------------------------------------------------------
 * Helper that tests the “quality Q” property for a 64‑bit number.
 * Return true if the number has at least one prime exponent ≥ 2
 * and at least two distinct prime factors.
 * ---------------------------------------------------------------- */
static bool has_quality_Q_uint64(uint64_t n)
{
    if (n <= 1) {
        return false;
    }

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    bool repeated = false;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] >= 2) {
            repeated = true;
            break;
        }
    }
    return repeated && cnt >= 2;
}

/* Wrapper that accepts the generic seq_t type.  All sequence values
   in this OEIS entry are representable as uint64_t, so the cast is
   safe for the required range. */
static bool has_quality_Q(seq_t x)
{
    return has_quality_Q_uint64((uint64_t)x);
}

/* ----------------------------------------------------------------
 * Main exported function.
 * It keeps a static cache of previously found terms; the cache is
 * expanded lazily to accommodate the requested index n.  The search
 * algorithm scans successive integers that are coprime to 6 (i.e.
 * congruent to 1 or 5 modulo 6); for each candidate it checks the
 * quality‑Q condition and the neighbor condition.  Once a term is
 * found it is stored in the cache and returned immediately.
 * ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                     /* safety for out‑of‑bounds calls */
    }

    /* Static cache of the sequence */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;
    static size_t cache_cap = 0;
    static uint64_t last_candidate = 1;   /* next integer to test */

    while (cache_sz < (size_t)n) {
        /* Search for the next valid k */
        uint64_t k = last_candidate;
        /* Skip numbers not coprime to 6 */
        while (k % 6 != 1 && k % 6 != 5) {
            ++k;
        }

        for (;;) {
            /* Check candidate k */
            if (has_quality_Q_uint64(k)) {
                bool ok = false;
                uint64_t km1 = k - 1;
                if (km1 > 1 && has_quality_Q_uint64(km1)) {
                    ok = true;
                }
                if (!ok) {
                    uint64_t kp1 = k + 1;
                    if (has_quality_Q_uint64(kp1)) {
                        ok = true;
                    }
                }
                if (ok) {
                    /* Found a term */
                    if (cache_sz == cache_cap) {
                        cache_cap = cache_cap ? cache_cap * 2 : 32;
                        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    }
                    cache[cache_sz++] = (seq_t)k;
                    last_candidate = k + 1;
                    break;
                }
            }
            /* Advance to next candidate coprime to 6 */
            ++k;
            while (k % 6 != 1 && k % 6 != 5) {
                ++k;
            }
        }
    }

    return cache[n - 1];
}
