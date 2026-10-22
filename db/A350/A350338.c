/*
 * A350338  Number of nontrivial divisors of n that are the product of up to 3
 *          (not necessarily distinct) primes.
 * https://oeis.org/A350338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [Omega(d) in {1,2,3}]
 * Range:     u8 n <= 9838, u16 n <= 9838, u32 n <= 9838, u64 n <= 9838,
 *            u128 n <= 9838, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of non‑trivial divisors of n that are the product of up to
   three (not necessarily distinct) primes. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0 */
        return 0;

    uint64_t m = (uint64_t)n;

    /* Factor n.  The maximum number of distinct prime factors < 64
       for any 64‑bit value, so static arrays of size 64 suffice. */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;

    factor_u64(m, primes, exps, &cnt);

    /* DP array: coeff[k] = number of divisors with Omega = k
       (k = 0..3).  Initially only the divisor 1 (Omega=0). */
    seq_t coeff[4] = { (seq_t)1, 0, 0, 0 };

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        seq_t newc[4] = { 0, 0, 0, 0 };

        for (int k = 0; k <= 3; ++k) {
            seq_t cur = coeff[k];
            if (!cur) continue;

            /* For this prime we may choose any exponent j from
               0 up to min(e, 3‑k).  Each choice contributes one
               divisor to the new Omega count. */
            for (int j = 0; j <= 3 - k && (uint64_t)j <= e; ++j)
                newc[k + j] += cur;
        }

        coeff[0] = newc[0];
        coeff[1] = newc[1];
        coeff[2] = newc[2];
        coeff[3] = newc[3];
    }

    /* Exclude the trivial divisor Omega=0. */
    return coeff[1] + coeff[2] + coeff[3];
}
