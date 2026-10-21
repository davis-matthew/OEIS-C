/*
 * A254932  Triangle used for the integral of odd powers of the sine and
 *          cosine functions.
 * https://oeis.org/A254932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = T(r,k) where
 *            T(n,k)=(-1)^(k+1)*binomial(2*n+1,n-k)*(2*n+1)!!/(2*k+1) and
 *            (r,k) is the pair in the triangular arrangement such that the
 *            linear index n (starting at 0) satisfies n = r*(r+1)/2 + k with
 *            0 <= k <= r
 * Range:     i8 n <= 2, i16 n <= 9, i32 n <= 27, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* provides binomial(idx_t,idx_t) */

/* Compute the odd double factorial (2*n+1)!! */
static seq_t odd_double_factorial(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= 2*n + 1; i += 2) {
        res *= (seq_t)i;
    }
    return res;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Find the row r such that (r+1)(r+2)/2 > n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }
    /* Column k is the offset within the row */
    idx_t k = n - r * (r + 1) / 2;

    /* Sign factor */
    seq_t sign = (k % 2 == 0) ? -1 : 1;

    /* Binomial coefficient: binomial(2*r+1, r-k) */
    seq_t bin = binomial(2 * r + 1, r - k);

    /* Odd double factorial (2*r+1)!! */
    seq_t odd_df = odd_double_factorial(r);

    /* Denominator (2*k+1) */
    seq_t denom = (2 * k + 1);

    /* Compute the triangle entry */
    seq_t result = bin * odd_df / denom;
    result *= sign;

    return result;
}
