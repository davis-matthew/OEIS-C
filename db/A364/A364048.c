/*
 * A364048  Expansion of Sum_{k>0} x^(5*k) / (1 + x^(6*k)).
 * https://oeis.org/A364048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, d==5 (mod 6)} (-1)^((d-5)/6)
 * Range:     i8 n <= 102, i16 n <= 102, i32 n <= 102, i64 n <= 102,
 *            i128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computes the value of OEIS sequence A364048 at index n.
   The computation is based on the divisor formula:
   a(n) = Σ_{d|n, d≡5 (mod 6)} (-1)^((d-5)/6)
   where the exponent is taken modulo 2 to decide the sign. */
seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;          /* Number to factorize. */
    seq_t result = 0;                   /* Accumulator for the sum. */

    for (uint64_t d = 1; d <= nn / d; ++d)   /* Iterate divisor pairs. */
    {
        if (nn % d == 0)                     /* d is a divisor of n. */
        {
            /* Process divisor d. */
            if (d % 6 == 5)
            {
                seq_t s = ((((d - 5) / 6) & 1) ? -1 : 1);
                result += s;
            }

            /* Process the complementary divisor e = n/d if distinct. */
            uint64_t e = nn / d;
            if (e != d && e % 6 == 5)
            {
                seq_t s = ((((e - 5) / 6) & 1) ? -1 : 1);
                result += s;
            }
        }
    }

    return result;
}
