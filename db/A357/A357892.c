/*
 * A357892  T(n,k) are the values of a variant of the Chebyshev polynomials
 *          P(n,x) of order n evaluated at x = k, where T(n,k), n >= 0, k <= n
 *          is a triangle read by rows. P(0,x) = 1, P(1,x) = x, P(n,x) =
 *          x*P(n-1,x) - P(n-2,x).
 * https://oeis.org/A357892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = P(row(n),offset(n)) where P(0,x)=1, P(1,x)=x,
 *            P(m,x)=x*P(m-1,x)-P(m-2,x). row(n) is the unique integer r with
 *            r*(r+1)/2 <= n < (r+1)*(r+2)/2. offset(n) = n - r*(r+1)/2
 * Range:     i8 n <= 13, i16 n <= 26, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n_idx)
{
    /* Find the row r that contains index n_idx in the flattened triangle. */
    uint64_t r = 0;
    while ( ((seq_t)(r+1)*((seq_t)(r+2)))/2 <= (seq_t)n_idx )
        ++r;

    /* Calculate offset k within that row. */
    seq_t start_row = ((seq_t)r * ((seq_t)(r+1)))/2;
    seq_t k = (seq_t)n_idx - start_row;      /* 0 <= k <= r */

    /* Evaluate the polynomial P(r,k). */
    if (r == 0)
        return (seq_t)1;
    if (r == 1)
        return k;

    seq_t prevprev = (seq_t)1;  /* P(0,k) */
    seq_t prev     = k;         /* P(1,k) */

    for (uint64_t i = 2; i <= r; ++i)
    {
        seq_t cur = k*prev - prevprev;  /* P(i,k) = k*P(i-1,k) - P(i-2,k) */
        prevprev = prev;
        prev = cur;
    }
    return prev;
}
