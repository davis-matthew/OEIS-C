/*
 * A293899  Number of proper divisors of the form 3k+1 minus number of proper
 *          divisors of the form 3k+2.
 * https://oeis.org/A293899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (#proper divisors d|n with d ≡ 1 (mod 3)) – (#proper
 *            divisors d|n with d ≡ 2 (mod 3))
 * Range:     i8 n <= 7856, i16 n <= 7856, i32 n <= 7856, i64 n <= 7856,
 *            i128 n <= 7856, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function factors n, builds a dynamic programming table for residues modulo 3,
   and overcounts all divisors. The contribution of n itself is then subtracted
   to obtain the value for proper divisors. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* factorize n */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    /* DP counts for residues modulo 3: counts[0], counts[1], counts[2] */
    seq_t counts[3] = { 0, 1, 0 };   /* start with divisor 1 (residue 1) */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        uint8_t pow_mod[65];          /* store p^a (mod 3) for a=0..e */
        uint8_t p_mod = p % 3U;
        uint8_t pow = 1;

        for (uint64_t a = 0; a <= e; ++a) {
            pow_mod[a] = pow;
            pow = (pow * p_mod) % 3U;
        }

        seq_t new_counts[3] = { 0, 0, 0 };

        for (int r0 = 0; r0 < 3; ++r0) {
            if (counts[r0] == 0) continue;
            seq_t base = counts[r0];
            for (uint64_t a = 0; a <= e; ++a) {
                int r1 = (r0 * pow_mod[a]) % 3;
                new_counts[r1] += base;
            }
        }

        counts[0] = new_counts[0];
        counts[1] = new_counts[1];
        counts[2] = new_counts[2];
    }

    /* remove the contribution of n itself (proper divisors only) */
    uint64_t mod3 = n % 3U;
    if (mod3 == 1) {
        counts[1] -= 1;
    } else if (mod3 == 2) {
        counts[2] -= 1;
    } else {
        counts[0] -= 1;
    }

    return counts[1] - counts[2];
}
