/*
 * A375034  The difference between the maximum odd exponent and the maximum
 *          even exponent in the prime factorization of n, where 0 is assigned
 *          to each maximum exponent if no such exponent exists.
 * https://oeis.org/A375034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*

   OEIS sequence A375034: a(n) = (maximum odd exponent in prime factorisation of n)
                      - (maximum even exponent in prime factorisation of n)
   If no odd (resp. even) exponent exists, its maximum is 0.

   Computation is done by trial division of n.  For the n values used in
   the tests (up to 10 000 at most) this is far below the time limit.
   The code works for any 64‑bit n because the loop condition uses
   `p <= temp / p` to avoid overflow.

   No external helper functions or libraries are needed except the
   standard headers that provide the integer types.

*/

#include <stdint.h>
#include <stdbool.h>

/*  External linkage, as required.  */
seq_t a(idx_t n)
{
    /* n is 1 for the first term */
    if (n <= 1U) return (seq_t)0;

    /* Work on a copy of n to isolate factorisation.  */
    idx_t temp = n;

    seq_t max_odd  = 0;
    seq_t max_even = 0;

    /* Trial division up to sqrt(temp).  */
    for (uint64_t p = 2U; p <= temp / p; ++p)
    {
        if (temp % p == 0U)
        {
            seq_t exp = 0;
            while (temp % p == 0U)
            {
                temp /= p;
                exp++;
            }
            if ((exp & 1U) != 0U)          /* odd exponent */
            {
                if (exp > max_odd) max_odd = exp;
            }
            else                           /* even exponent */
            {
                if (exp > max_even) max_even = exp;
            }
        }
    }

    /* If something remains, it's a prime factor with exponent 1. */
    if (temp > 1U)
    {
        /* exponent is 1, which is odd */
        if (1 > max_odd) max_odd = 1;
    }

    return max_odd - max_even;
}
