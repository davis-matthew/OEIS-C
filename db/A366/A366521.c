/*
 * A366521  Largest squarefree divisor of n which is <= sqrt(n).
 * https://oeis.org/A366521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ d : d|n, d squarefree, d ≤ √n }
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9511 (2026-11-01)
 * Helpers:   factor_u64, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper functions factor_u64 and floor_sqrt from utils */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* for safety, though n≥1 */

    /* Floor of √n stored in the same type as the result */
    const seq_t sq = floor_sqrt((seq_t)n);

    /* Factor n; only the distinct primes are needed */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t best = 1;                            /* 1 is always square‑free */

    /* Enumerate all non‑empty subsets of the prime factors. */
    /* For n ≤ 10 000 the number of distinct primes is ≤ 5, so 64‑bit
       masks are safe. */
    uint64_t total_masks = 1ULL << pcnt;
    for (uint64_t mask = 1; mask < total_masks; ++mask) {
        seq_t d = 1;
        for (size_t i = 0; i < pcnt; ++i) {
            if (mask & (1ULL << i)) {
                d *= (seq_t)primes[i];
            }
        }
        if (d <= sq && d > best) best = d;
    }

    return best;
}
