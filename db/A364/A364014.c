/*
 * A364014  Expansion of Sum_{k>0} x^(2*k) / (1 + x^(3*k)).
 * https://oeis.org/A364014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, d≡2 (mod 3)} (-1)^d
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate over all divisors d of n and add (-1)^d
   whenever d ≡ 2 (mod 3). Uses only integer arithmetic. */

#include <stdbool.h>  /* for bool type if needed */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, but guard against misuse */

    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d)   /* iterate d up to sqrt(n) safely */
    {
        if (n % d == 0)
        {
            /* divisor d */
            if (d % 3 == 2)
                sum += (d & 1) ? -1 : 1;  /* (-1)^d */

            idx_t d2 = n / d;
            if (d2 != d)                 /* complementary divisor */
            {
                if (d2 % 3 == 2)
                    sum += ((d2 & 1) ? -1 : 1);
            }
        }
    }
    return sum;
}
