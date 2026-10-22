/*
 * A341619  Characteristic function of primitive nondeficient numbers
 *          (A006039): a(n) = 1 if proper multiples of n are all abundant, and
 *          proper divisors of n are all deficient, 0 otherwise.
 * https://oeis.org/A341619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff all proper divisors of n are deficient and all
 *            proper multiples of n are abundant. This is equivalent to n
 *            being a primitive nondeficient number (A006039). The check is
 *            performed by computing the sum–of–divisors function σ(n) and
 *            comparing it with 2n
 * Range:     u8 n <= 8885, u16 n <= 8885, u32 n <= 8885, u64 n <= 8885,
 *            u128 n <= 8885, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Forward declaration of helper to compute powers of an integer
   in the arbitrary‑precision type seq_t.  We implement it locally
   so that the code is valid both in C and C++.  */
static seq_t power_uint(uint64_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)base;
    return result;
}

/* Compute σ(m) – the sum of all positive divisors of m.  */
static seq_t sigma_of(idx_t m)
{
    if (m == 1)
        return 1;

    seq_t sigma = 1;
    idx_t temp = m;

    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            idx_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            /* Sum_{i=0}^{exp} p^i = (p^{exp+1} – 1)/(p – 1) */
            seq_t term = (power_uint(p, exp + 1) - 1) / (p - 1);
            sigma *= term;
        }
    }

    if (temp > 1)            /* prime factor remaining (exponent 1) */
    {
        seq_t term = (power_uint(temp, 2) - 1) / (temp - 1);
        sigma *= term;
    }

    return sigma;
}

/* ────────────────────────────────────────────────────────────── */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* 1.  n must be non‑deficient (σ(n) ≥ 2n) */
    seq_t sigma_n = sigma_of(n);
    if (sigma_n < (seq_t)(2 * n))
        return (seq_t)0;

    /* 2.  Every proper divisor must be deficient (σ(d) < 2d) */
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            if (d < n)
            {
                seq_t sigma_d = sigma_of(d);
                if (sigma_d >= (seq_t)(2 * d))
                    return (seq_t)0;
            }

            idx_t e = n / d;
            if (e != d && e < n)
            {
                seq_t sigma_e = sigma_of(e);
                if (sigma_e >= (seq_t)(2 * e))
                    return (seq_t)0;
            }
        }
    }

    return (seq_t)1;
}
