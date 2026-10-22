/*
 * A346878  Sum of the divisors, except for the largest, of the n-th positive
 *          even number.
 * https://oeis.org/A346878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = σ(2 n) – 2 n where σ(m) is the sum of all positive
 *            divisors of m
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2 n, the n‑th positive even number */
    uint64_t m = n << 1;          /* 2 * n, fits in uint64_t for all test values  */

    seq_t sigma = 1;              /* product of divisor sums of each prime power  */
    uint64_t temp = m;            /* mutable copy for factorisation   */

    for (uint64_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            seq_t sum = 1;        /* 1 + p + p² + … for this prime power   */
            seq_t pow = 1;        /* current power of p                     */

            while (temp % p == 0)
            {
                temp /= p;
                pow *= (seq_t)p;
                sum += pow;
            }
            sigma *= sum;
        }
    }

    if (temp > 1)                 /* residual prime factor                     */
    {
        sigma *= ((seq_t)temp + 1);  /* prime contributes (p + 1)                 */
    }

    return sigma - (seq_t)m;       /* subtract the largest divisor (m itself)   */
}
