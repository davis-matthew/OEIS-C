/*
 * A378665  Greatest non-abundant divisor of n.
 * https://oeis.org/A378665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 100 terms through n = 8756 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A378665: the greatest divisor d of n such that
   d is not abundant, i.e. σ(d) ≤ 2·d.  The value is returned as the
   type seq_t.  n is given as idx_t (unsigned 64‑bit) and must be
   ≥ 1.  The algorithm factors n, enumerates all its divisors, and
   keeps the largest divisor whose sum–of–divisors does not exceed
   twice its value.  The enumeration is done recursively, computing
   σ(d) incrementally to stay fast.  All intermediate arithmetic fits
   within 64 bits for the range of n considered. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Recursive search for the maximum non‑abundant divisor.
   primes[] and exps[] hold the prime factorisation of n.
   curr_d      current divisor value (≤ n).
   curr_sigma  current value of σ(curr_d).
   best        pointer to the best divisor found so far. */
static void search_divs(const uint64_t *primes, const uint64_t *exps,
                        size_t cnt, size_t idx, uint64_t curr_d,
                        uint64_t curr_sigma, uint64_t *best)
{
    if (idx == cnt) {
        /* At a leaf: test whether d is non‑abundant. */
        uint64_t limit = curr_d * 2ULL;          /* 2·d  */
        if (curr_sigma <= limit && curr_d > *best)
            *best = curr_d;
        return;
    }

    uint64_t p   = primes[idx];
    uint64_t emax = exps[idx];
    uint64_t pow   = 1;          /* p^e */
    uint64_t sum_p   = 1;        /* 1 + p + ... + p^e */

    for (uint64_t e = 0; e <= emax; ++e) {
        /* Recurse with the current exponent e. */
        search_divs(primes, exps, cnt, idx + 1,
                    curr_d * pow, curr_sigma * sum_p, best);

        /* Prepare for the next exponent.  Skip update after last loop. */
        if (e < emax) {
            pow   *= p;
            sum_p += pow;
        }
    }
}

/* Public sequence function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined offset, graceful fallback */

    /* Factorise n.  The array size 64 is ample for n ≤ 2^64‑1. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t best_div = 1;       /* 1 is always non‑abundant */
    search_divs(primes, exps, cnt, 0, 1ULL, 1ULL, &best_div);

    return (seq_t)best_div;
}
