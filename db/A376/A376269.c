/*
 * A376269  a(n) = n! + (n - 1)! + (n - 2)! + n - 3.
 * https://oeis.org/A376269
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = factorial(n-2) * n^2 + n - 3 (for n ≥ 2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If n < 2 the function returns 0 (the offset is 2). */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset. */
    if (n < 2u)
        return (seq_t)0;

    /* Compute (n-2)! using the provided factorial helper. */
    seq_t fac = factorial(n - 2);

    /* Convert n to seq_t for arithmetic. */
    seq_t nn = (seq_t)n;

    /* a(n) = fac * n^2 + n - 3  */
    seq_t val = fac * nn * nn;   /* (n-2)! * n^2   */
    val += nn;                   /* + n            */
    val -= (seq_t)3;             /* - 3            */

    return val;
}
