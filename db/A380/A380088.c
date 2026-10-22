/*
 * A380088  The largest unitary divisor of n that is a term in A207481.
 * https://oeis.org/A380088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (p^e if e ≤ p, otherwise 1)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the largest unitary divisor of n that is a term of A207481. */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1 – for n = 0 return 0 as a guard. */
    if (n == 0) return 0;

    /* factor_u64 works on uint64_t, and idx_t is guaranteed to be uint64_t above. */
    uint64_t nn = (uint64_t)n;

    /* Arrays big enough to hold all prime factors of a 64‑bit integer. */
    size_t   cnt = 0;
    uint64_t pr[64];
    uint64_t ex[64];

    factor_u64(nn, pr, ex, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];

        if (e <= p) {
            seq_t pow = 1;
            for (uint64_t j = 0; j < e; ++j) {
                pow *= (seq_t)p;
            }
            result *= pow;
        }
        /* when e > p the term contributes 1, so nothing to do */
    }

    return result;
}
