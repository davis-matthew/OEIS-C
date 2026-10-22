/*
 * A344854  The number of equilateral triangles with vertices from the
 *          vertices of the n-dimensional hypercube.
 * https://oeis.org/A344854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n*Sum_{k=1..floor(n/3)} n!/(6*(n - 3*k)!*k!^3)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    idx_t maxk = n / 3;
    seq_t sum = 0;

    for (idx_t k = 1; k <= maxk; ++k)
    {
        seq_t term = binom(n, 3 * k);
        term = term * binom(3 * k, k);
        term = term * binom(2 * k, k);
        term = term / 6;            /* exact division */
        sum = sum + term;
    }

    seq_t pow2 = pow_uint(2, n);
    return sum * pow2;
}
