/*
 * A245092  The even numbers (A005843) and the values of sigma function
 *          (A000203) interleaved.
 * https://oeis.org/A245092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 3186, u32 n <= 9714, u64 n <= 9714,
 *            u128 n <= 9714, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(2k) = 2k  
 *          a(2k+1) = σ(k+1)  (sum of divisors of k+1)
 */

#include <stdint.h>

/* Compute x^exp for seq_t using repeated squaring */
static seq_t power_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Sum of divisors σ(n) for n >= 1 */
static seq_t sigma_of(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;   /* σ(0) and σ(1) are defined as 1 for this use */

    seq_t sigma = (seq_t)1;
    idx_t m = n;

    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            idx_t exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            seq_t term = (power_seq((seq_t)p, exp + 1) - (seq_t)1)
                         / ((seq_t)p - (seq_t)1);
            sigma *= term;
        }
    }

    if (m > 1)   /* remaining prime factor */
    {
        seq_t term = (power_seq((seq_t)m, (idx_t)2) - (seq_t)1)
                     / ((seq_t)m - (seq_t)1);
        sigma *= term;
    }

    return sigma;
}

/* OEIS A245092 */
seq_t a(idx_t n)
{
    if ((n & (idx_t)1) == 0)          /* even index */
        return (seq_t)n;              /* even number 0,2,4,… */
    else                              /* odd index */
        return sigma_of((n + 1) / 2);
}
