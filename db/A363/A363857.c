/*
 * A363857  Number of divisors of 7*n-5 of form 7*k+6.
 * https://oeis.org/A363857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A363857:  Number of divisors of 7*n-5 that are congruent to 6 (mod 7).
 * This implementation factors 7*n-5 and counts divisor residues modulo 7
 * using dynamic programming over all prime power factors.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 7*n-5 fits in 64 bits for all practical n.  */
    uint64_t M = 7ULL * n - 5ULL;

    uint64_t m = M;
    uint64_t primes[64];
    size_t exps[64];
    size_t nf = 0;

    /* Trial‑division factorisation.  */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            size_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[nf] = p;
            exps[nf] = e;
            ++nf;
        }
    }
    if (m > 1) {
        primes[nf] = m;
        exps[nf] = 1;
        ++nf;
    }

    /* dp[r] = number of divisor combinations giving residue r (mod 7).  */
    seq_t dp[7] = {0};
    dp[1] = 1; /* the divisor 1 has residue 1 modulo 7 */

    for (size_t i = 0; i < nf; ++i) {
        seq_t prime_counts[7] = {0};
        uint64_t modp = primes[i] % 7ULL;   /* residue of the prime itself */

        int residue = 1;                     /* residue of p^0 */
        for (size_t e = 0; e <= exps[i]; ++e) {
            prime_counts[residue] += (seq_t)1;
            residue = (residue * (int)modp) % 7;
        }

        seq_t newdp[7] = {0};
        for (int a = 0; a < 7; ++a) {
            if (dp[a] == 0) continue;
            for (int b = 0; b < 7; ++b) {
                if (prime_counts[b] == 0) continue;
                seq_t prod = dp[a] * prime_counts[b];
                int idx = (a * b) % 7;
                newdp[idx] += prod;
            }
        }
        for (int r = 0; r < 7; ++r) dp[r] = newdp[r];
    }

    return dp[6];   /* divisors ≡ 6 (mod 7) */
}
