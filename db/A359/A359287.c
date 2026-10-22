/*
 * A359287  Number of divisors of 5*n-1 of form 5*k+2.
 * https://oeis.org/A359287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of (5*n-1) that are congruent to 2
 *            modulo 5
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* 5*n - 1 fits in uint64_t for any n up to 2^63-1 */
    uint64_t m = 5 * (uint64_t)n - 1;

    /* factor_u64 returns primes and exponents (unsigned long long) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(m, primes, exps, &cnt);

    /* dynamic programming over residues modulo 5 */
    seq_t counts[5] = {0};
    counts[1] = 1;                     /* divisor 1 contributes residue 1 */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        unsigned int e = (unsigned int)exps[i];

        seq_t newc[5] = {0};

        unsigned int pk_mod5 = 1;      /* p^k mod 5, starting with k = 0 */
        for (unsigned int k = 0; k <= e; ++k) {
            for (int r = 0; r < 5; ++r) {
                int nr = (r * pk_mod5) % 5;
                newc[nr] += counts[r];
            }
            if (k < e) {
                pk_mod5 = (pk_mod5 * (unsigned int)(p % 5)) % 5;
            }
        }

        /* copy newc back to counts */
        for (int r = 0; r < 5; ++r) {
            counts[r] = newc[r];
        }
    }

    return counts[2];
}
