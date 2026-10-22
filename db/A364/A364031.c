/*
 * A364031  Expansion of Sum_{k>0} k * x^k / (1 + x^(4*k)).
 * https://oeis.org/A364031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d==1 (mod 4)} (-1)^((d-1)/4) * (n/d)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>   /* for size_t if needed */

seq_t a(idx_t n)
{
    seq_t pos = 0;
    seq_t neg = 0;

    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            /* divisor d */
            if ((d & 3) == 1)
            {
                seq_t term = (seq_t)(n / d);
                if (((d - 1) >> 2) & 1)
                    neg += term;
                else
                    pos += term;
            }

            /* corresponding divisor n/d */
            idx_t d2 = n / d;
            if (d2 != d && (d2 & 3) == 1)
            {
                seq_t term = (seq_t)(n / d2);  /* equals d */
                if (((d2 - 1) >> 2) & 1)
                    neg += term;
                else
                    pos += term;
            }
        }
    }

    /* the sum is non‑negative for all n */
    return pos - neg;
}
