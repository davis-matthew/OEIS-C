/*
 * A320059  Sum of divisors of n^2 that do not divide n.
 * https://oeis.org/A320059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 149, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sigma(n^2) – sigma(n)
 * where sigma(m) = sum of all positive divisors of m.
 */

#include <stdbool.h>
#include <stdint.h>

/* fast exponentiation for seq_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* defined for n >= 1 only */

    seq_t sigma_n  = 1;                /* sigma(n)   */
    seq_t sigma_n2 = 1;                /* sigma(n²)  */

    idx_t m = n;                       /* temporary copy of n */

    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p != 0) continue;

        /* find exponent of prime p in n */
        idx_t e = 0;
        while (m % p == 0)
        {
            m /= p;
            ++e;
        }

        /* term for sigma(n)   : (p^(e+1)-1)/(p-1)  */
        seq_t term_sigma   = (pow_seq((seq_t)p, e + 1) - 1)
                            / ((seq_t)(p - 1));

        /* term for sigma(n²) : (p^(2e+1)-1)/(p-1) */
        seq_t term_sigma2  = (pow_seq((seq_t)p, 2 * e + 1) - 1)
                            / ((seq_t)(p - 1));

        sigma_n  *= term_sigma;
        sigma_n2 *= term_sigma2;
    }

    /* remaining prime factor > sqrt(n) */
    if (m > 1)
    {
        seq_t p = m;                    /* exponent e = 1 */
        seq_t term_sigma   = p + 1;                    /* 1 + p          */
        seq_t term_sigma2  = p * p + p + 1;            /* 1 + p + p²     */
        sigma_n  *= term_sigma;
        sigma_n2 *= term_sigma2;
    }

    return sigma_n2 - sigma_n;
}
