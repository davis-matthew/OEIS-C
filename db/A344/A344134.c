/*
 * A344134  a(n) = Sum_{i|n, j|n, k|n} lcm(i,j,k).
 * https://oeis.org/A344134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^a∥n} Σ_{M=0}^a p^M · (((M+1)^3 – M^3))
 * Range:     u8 n <= 5, u16 n <= 59, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* offset starts at 1, but guard for completeness */
        return 0;
    seq_t result = 1;
    idx_t temp = n;

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
            seq_t pow = 1;          /* p^M */
            seq_t Sp  = 0;          /* Σ p^M · ((M+1)^3 – M^3) */
            for (idx_t M = 0; M <= exp; ++M)
            {
                if (M > 0)
                    pow *= (seq_t)p;   /* update p^M */
                seq_t diff = ((M + 1) * (M + 1) * (M + 1) - M * M * M);
                Sp += pow * diff;
            }
            result *= Sp;
        }
    }
    if (temp > 1)        /* remaining prime factor */
    {
        idx_t exp = 1;
        seq_t pow = 1;
        seq_t Sp  = 0;
        for (idx_t M = 0; M <= exp; ++M)
        {
            if (M > 0)
                pow *= (seq_t)temp;
            seq_t diff = ((M + 1) * (M + 1) * (M + 1) - M * M * M);
            Sp += pow * diff;
        }
        result *= Sp;
    }
    return result;
}
