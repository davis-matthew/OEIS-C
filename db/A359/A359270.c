/*
 * A359270  Number of divisors of 5*n-3 of form 5*k+3.
 * https://oeis.org/A359270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the number of divisors of (5*n-3) that are congruent to 3
 *            modulo 5
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (this is equivalently the number of divisors congruent to 4 modulo 5) */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Compute m = 5*n – 3 without overflow. */
    uint64_t m = 5ull * n - 3ull;          /* 5*n-3 */

    /* Factorise m.  The helper uses uint64_t arrays for primes and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;

    factor_u64(m, primes, exps, &cnt);

    /* dp[r] will hold the number of divisor products with residue r (mod 5). */
    seq_t dp[5] = {0};
    dp[1] = (seq_t)1;          /* divisor 1 has residue 1 mod 5 */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t newdp[5] = {0};
        uint64_t p_mod5 = primes[i] % 5;
        uint64_t pow_mod5 = 1;               /* p_mod5^e % 5 for e = 0 .. exps[i] */

        for (uint64_t e = 0; e <= exps[i]; ++e) {
            for (int r = 0; r < 5; ++r) {
                if (dp[r] != 0) {
                    int newr = (r * (int)pow_mod5) % 5;
                    newdp[newr] += dp[r];
                }
            }
            pow_mod5 = (pow_mod5 * p_mod5) % 5;
        }
        for (int r = 0; r < 5; ++r)
            dp[r] = newdp[r];
    }

    return dp[3];
}
