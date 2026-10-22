/*
 * A378665  Greatest non-abundant divisor of n.
 * https://oeis.org/A378665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8756, u32 n <= 8756, u64 n <= 8756,
 *            u128 n <= 8756, big any n
 * Helpers:   factor_u64, search_divs
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

#include "../utils/factor_u64.h"    /* provides factor_u64 */
#include "../utils/search_divs.h"

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
