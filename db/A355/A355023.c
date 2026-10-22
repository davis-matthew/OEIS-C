/*
 * A355023  Number of labeled trees on n nodes with maximum degree three and
 *          three vertices of degree three.
 * https://oeis.org/A355023
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * binomial(n,8) * factorial(n-2)
 * Range:     u8 none, u16 n <= 8, u32 n <= 11, u64 n <= 18, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 8.  For below that, return 0. */
    if (n < 8)
        return 0;

    /* Compute C(n,8) and multiply by 7. */
    seq_t res = binomial(n, 8);
    res = res * (seq_t)7;

    /* Multiply by (n-2)! . */
    res = res * factorial(n - 2);

    return res;
}
