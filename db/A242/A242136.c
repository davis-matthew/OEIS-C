/*
 * A242136  Number of strong triangulations of a fixed square with n interior
 *          vertices.
 * https://oeis.org/A242136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(4*n+3, n-1) - 5*binomial(4*n+3, n-2) +
 *            6*binomial(4*n+3, n-3) (a(0)=0)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* 4*n+3 is safe for n up to the limits of the sequence (e.g., 10 000). */
    idx_t tot = 4 * n + 3;
    seq_t c1 = binomial(tot, n - 1);

    seq_t c2 = (n >= 2) ? binomial(tot, n - 2) : (seq_t)0;
    seq_t c3 = (n >= 3) ? binomial(tot, n - 3) : (seq_t)0;

    seq_t result = c1;
    result -= (seq_t)5 * c2;
    result += (seq_t)6 * c3;
    return result;
}
