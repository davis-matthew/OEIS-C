/*
 * A387717  Greatest deficient divisor of n.
 * https://oeis.org/A387717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = max{ d | d divides n and sigma(d) < 2*d } (OEIS A387717)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 100 terms through n = 9552 (2026-11-03)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

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

/* Recursive search for the largest deficient divisor.
   Parameters:
     pr   – prime factors of n
     ex   – corresponding exponents
     idx  – current prime index
     cur  – current divisor value (uint64_t, ≤ n)
     sigma – sum of divisors of cur (seq_t)
   The current best deficient divisor is stored in *best. */
static void search(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                   size_t idx, uint64_t cur, seq_t sigma, seq_t *best)
{
    if (idx == cnt) {
        /* cur is a divisor of the original n */
        if (sigma < (seq_t)2 * (seq_t)cur && cur > (uint64_t)*best)
            *best = (seq_t)cur;
        return;
    }

    uint64_t p = pr[idx];
    uint64_t pow_p = 1;     /* p^e for e = 0 .. ex[idx] */
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        /* sigma factor for this exponent e:  (p^(e+1)-1)/(p-1) */
        uint64_t term_num = pow_p * p - 1;
        uint64_t term_den = p - 1;
        seq_t term = (seq_t)(term_num / term_den);
        search(pr, ex, cnt, idx + 1, cur * pow_p, sigma * term, best);
        pow_p *= p;
    }
}

/* Static cache for memoisation; 0 means "not computed" (a(n) ≥ 1 for all n). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Main sequence function.  n ≥ 1.  Uses memoisation and a divisor search. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined below offset */

    /* Ensure cache space for index n. */
    ensure_cache(n, &cache, &cache_cap);

    if (cache[n - 1] != 0)                  /* already computed */
        return cache[n - 1];

    /* Factorise n.  64 slots are more than enough for n ≤ 100 000. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t best = 0;
    /* Start with divisor 1 (sigma = 1). */
    search(primes, exps, cnt, 0, 1, (seq_t)1, &best);

    cache[n - 1] = best;
    return best;
}
