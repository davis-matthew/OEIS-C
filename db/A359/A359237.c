/*
 * A359237  Number of divisors of 5*n-3 of form 5*k+1.
 * https://oeis.org/A359237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of (5*n-3) that are ≡ 1 (mod 5)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9996 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Count divisors of the product ∏ p_i^e_i that are ≡1 (mod 5).  Any
   divisor containing the prime 5 is automatically 0 (mod 5) and is
   therefore ignored. */
static seq_t count_div_mod1(const uint64_t *pr, const uint64_t *ex, size_t cntf)
{
    seq_t dp[5] = {0};
    dp[1 % 5]   = 1;                /* divisor 1 */

    for (size_t i = 0; i < cntf; ++i) {
        uint64_t p = pr[i];
        if (p == 5u)                 /* skip any factor 5 */
            continue;

        uint8_t pmod = p % 5u;
        /* powers of pmod modulo 5 up to the exponent */
        uint8_t powp[65];
        powp[0] = 1 % 5u;
        for (size_t k = 1; k <= ex[i]; ++k)
            powp[k] = (powp[k - 1] * pmod) % 5u;

        seq_t next[5] = {0};

        for (int r = 0; r < 5; ++r) {
            if (dp[r] == 0)
                continue;
            seq_t ways = dp[r];
            for (size_t k = 0; k <= ex[i]; ++k) {
                int newr = (r * powp[k]) % 5;
                next[newr] += ways;
            }
        }
        for (int r = 0; r < 5; ++r)
            dp[r] = next[r];
    }
    return dp[1 % 5];
}

/* Simple trial‐division factorisation for 64‑bit unsigned integers. */
static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[c]   = p;
            ex[c]   = e;
            ++c;
        }
    }
    if (n > 1) {            /* remaining prime factor */
        pr[c]   = n;
        ex[c]   = 1;
        ++c;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset 1 – undefined for n=0 */
        return 0;

    uint64_t m64 = 5 * n - 3;    /* m = 5*n - 3 */

    uint64_t primes[64];
    uint64_t exponents[64];
    size_t fact_cnt = 0;

    factor_u64(m64, primes, exponents, &fact_cnt);

    return count_div_mod1(primes, exponents, fact_cnt);
}
