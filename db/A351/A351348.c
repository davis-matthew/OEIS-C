/*
 * A351348  Dirichlet g.f.: Product_{p prime} (1 + 2*p^(-s)) / (1 - p^(-s) -
 *          p^(-2*s)).
 * https://oeis.org/A351348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} Lucas(e+1) (Lucas numbers: L0 = 2, L1 = 1, Lk =
 *            Lk-1 + Lk-2)
 * Range:     u8 n <= 959, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 8771 (2026-11-03)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

static seq_t lucas(idx_t k)
{
    if (k == 0) return (seq_t)2;
    if (k == 1) return (seq_t)1;
    seq_t Lprev = (seq_t)2; /* L0 */
    seq_t L = (seq_t)1;      /* L1 */
    for (idx_t i = 2; i <= k; ++i) {
        seq_t tmp = L + Lprev;
        Lprev = L;
        L = tmp;
    }
    return L;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but guard */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Ensure cache can hold index n */
    if (n >= cache_cap) {
        ensure_cache(n, &cache, &cache_cap);
    }

    if (cache[n] != 0) {
        return cache[n];
    }

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t e = (idx_t)exps[i];
        /* Lucas(e+1) */
        seq_t L = lucas(e + 1);
        result *= L;
    }

    cache[n] = result;
    return result;
}
