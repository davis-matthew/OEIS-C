/*
 * A359238  Number of divisors of 5*n-4 of form 5*k+1.
 * https://oeis.org/A359238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of 5*n-4 that are congruent to 1 (mod
 *            5)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* 5*n-4 is always congruent to 1 mod 5 */
    uint64_t N = (uint64_t)5 * n - 4;

    /* factor the integer N */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(N, primes, exps, &cnt);

    /* dp[r] = number of partial divisors producing residue r (mod 5) */
    seq_t dp[5] = {0};
    dp[1] = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        size_t e = exps[i];

        /* pre‑compute p^k mod 5 for k = 0 … e */
        uint64_t pow_mod[64];
        pow_mod[0] = 1 % 5;
        for (size_t k = 1; k <= e; ++k)
            pow_mod[k] = (pow_mod[k - 1] * (p % 5)) % 5;

        seq_t newdp[5] = {0};
        for (int r = 0; r < 5; ++r) {
            if (dp[r] == 0) continue;
            for (size_t k = 0; k <= e; ++k) {
                int nr = (r * pow_mod[k]) % 5;
                newdp[nr] += dp[r];
            }
        }
        for (int r = 0; r < 5; ++r)
            dp[r] = newdp[r];
    }

    return dp[1];
}
