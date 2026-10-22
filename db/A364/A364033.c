/*
 * A364033  Expansion of Sum_{k>0} k * x^(3*k) / (1 + x^(4*k)).
 * https://oeis.org/A364033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, d==3 (mod 4)} (-1)^((d-3)/4) * (n/d)
 * Range:     i8 n <= 91, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;

            if (d1 % 4 == 3)
            {
                seq_t sign = ((d1 - 3) / 4) & 1 ? (seq_t)-1 : (seq_t)1;
                result += sign * ((seq_t)n / d1);
            }

            if (d2 != d1 && d2 % 4 == 3)
            {
                seq_t sign = ((d2 - 3) / 4) & 1 ? (seq_t)-1 : (seq_t)1;
                result += sign * ((seq_t)n / d2);
            }
        }
    }
    return result;
}
