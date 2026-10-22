/*
 * A359269  Number of divisors of 5*n-2 of form 5*k+2.
 * https://oeis.org/A359269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors d of 5*n-2 with d mod 5 = 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 5*n-2; n is small enough that 5*n fits in uint64_t */
    uint64_t m = (uint64_t)5 * n - 2;
    if (m == 0) return 0;

    /* Factorise m by trial division */
    uint64_t tmp = m;
    uint64_t primes[16];
    unsigned exps[16];
    size_t pc = 0;
    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            unsigned e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            primes[pc] = p;
            exps[pc] = e;
            ++pc;
        }
    }
    if (tmp > 1)
    {
        primes[pc] = tmp;
        exps[pc] = 1;
        ++pc;
    }

    /* DP on residues modulo 5 */
    seq_t dp[5] = {0};
    dp[1 % 5] = 1;
    for (size_t i = 0; i < pc; ++i)
    {
        uint64_t p = primes[i];
        unsigned e = exps[i];
        seq_t newdp[5] = {0};

        /* Compute p^k mod 5 for k = 0..e and update DP */
        uint64_t pow = 1;
        for (unsigned k = 0; k <= e; ++k)
        {
            unsigned residue = (unsigned)(pow % 5);
            for (int r = 0; r < 5; ++r)
            {
                if (dp[r] == 0) continue;
                unsigned prod = (r * residue) % 5;
                newdp[prod] += dp[r];
            }
            pow *= p;
        }

        /* Copy newdp to dp */
        for (int r = 0; r < 5; ++r) dp[r] = newdp[r];
    }

    return dp[2 % 5];
}
