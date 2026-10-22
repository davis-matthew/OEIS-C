/*
 * A363887  Number of divisors of 7*n-5 of form 7*k+3.
 * https://oeis.org/A363887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of (7*n-5) with d ≡ 3 (mod 7)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* trial factorisation of a 64‑bit integer into primes and exponents */
static void factor_uint64(uint64_t n, uint64_t primes[64], uint64_t exps[64], size_t *cnt)
{
    size_t c = 0;
    while ((n & 1ULL) == 0ULL) {
        uint64_t e = 0;
        while ((n & 1ULL) == 0ULL) {
            n >>= 1;
            e++;
        }
        primes[c] = 2ULL;
        exps[c] = e;
        c++;
    }
    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL) {
        if (n % p == 0ULL) {
            uint64_t e = 0;
            while (n % p == 0ULL) {
                n /= p;
                e++;
            }
            primes[c] = p;
            exps[c] = e;
            c++;
        }
    }
    if (n > 1ULL) {
        primes[c] = n;
        exps[c] = 1ULL;
        c++;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    /* Compute m = 7*n - 5 */
    uint64_t m = (uint64_t)7ULL * (uint64_t)n - 5ULL;

    /* All factors of 7 can be removed – such factors never contribute
       to a divisor congruent to 3 (mod 7). */
    while ((m % 7ULL) == 0ULL) {
        m /= 7ULL;
    }

    if (m == 1ULL) {
        return 0;
    }

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_uint64(m, primes, exps, &cnt);

    /* dp[r] – number of divisors of the processed part having residue r modulo 7 */
    seq_t dp[7] = {0};
    dp[1] = 1;   /* divisor 1 has residue 1 */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* precompute p^f (mod 7) for f = 0..e */
        uint8_t modpow[65];   /* e ≤ 64 for 64‑bit numbers */
        modpow[0] = 1 % 7;
        for (uint64_t f = 1; f <= e; ++f) {
            modpow[f] = (uint8_t)((modpow[f - 1] * (p % 7ULL)) % 7ULL);
        }

        seq_t newdp[7] = {0};
        for (int r = 0; r < 7; ++r) {
            if (dp[r] == 0) continue;
            for (uint64_t f = 0; f <= e; ++f) {
                int nr = (r * modpow[f]) % 7;
                newdp[nr] = newdp[nr] + dp[r];
            }
        }
        for (int r = 0; r < 7; ++r) {
            dp[r] = newdp[r];
        }
    }

    return dp[3];
}
