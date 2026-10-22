/*
 * A394481  a(n) = usigma(n) + tau(n) - tau(rad(n)).
 * https://oeis.org/A394481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n} [gcd(d,n/d)=1] * d + [gcd(d,n/d)≠1] * 1
 *            Equivalent: a(n) = usigma(n) + tau(n) - tau(rad(n))
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* local fast integer power – result fits in seq_t */
static inline seq_t power_uint(uint64_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result = result * (seq_t)base;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety guard – sequence starts at n=1 */

    /* factor n – n ≤ 10 000 in typical tests, trial division is fine */
    uint64_t primes[64];           /* prime bases */
    uint64_t exps[64];             /* corresponding exponents */
    size_t   cnt = 0;              /* number of distinct primes */

    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            uint64_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            primes[cnt] = (uint64_t)p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (temp > 1)                 /* remaining prime factor */
    {
        primes[cnt] = temp;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* compute the three terms of the formula */
    seq_t sigma_u = 1;            /* product (1 + p^e) */
    seq_t tau_n   = 1;            /* product (e+1)          */
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p_pow = power_uint(primes[i], exps[i]);   /* p^e */
        sigma_u *= (seq_t)1 + p_pow;
        tau_n   *= (seq_t)(exps[i] + 1);
    }

    seq_t pow2 = (seq_t)1 << cnt; /* 2^omega(n) where omega = cnt */

    return sigma_u + tau_n - pow2;
}
