/*
 * A366521  Largest squarefree divisor of n which is <= sqrt(n).
 * https://oeis.org/A366521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ d : d|n, d squarefree, d ≤ √n }
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper functions factor_u64 and floor_sqrt from utils */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/floor_sqrt.h"

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
