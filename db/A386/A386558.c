/*
 * A386558  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals: A(n,k)
 *          = binomial((k+1)*n+k-1,n)/(n+1).
 * https://oeis.org/A386558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the value obtained by reading the square array A(i,j) =
 *            binomial((j+1)*i + j - 1, i) / (i + 1) along antidiagonals. For
 *            index n, let d be the unique integer for which d*(d+1)/2 ≤ n <
 *            (d+1)*(d+2)/2 (1) Then i = n - d*(d+1)/2 and j = d - i. The
 *            wanted term is a(n) = A(i,j) (with A(0,j)=1 an...
 * Range:     u8 n <= 17, u16 n <= 40, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility function: binomial is provided in utils. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the diagonal index d.  Use an incremental search,
       which is still fast for n up to many millions. */
    idx_t d = 0;                      /* current diagonal (starting 0)   */
    idx_t acc = 1;                    /* acc = (d+1)(d+2)/2  for current d */
    while (acc <= n) {
        ++d;
        acc += d + 1;                 /* advance to next diagonal */
    }

    idx_t prevAcc = acc - (d + 1);    /* acc = (d+1)(d+2)/2, so previous acc = d*(d+1)/2 */
    idx_t i = n - prevAcc;            /* row index within diagonal   */
    idx_t j = d - i;                  /* column index within diagonal */

    /* Handle the special rows/columns that always give 0 or 1. */
    if (i == 0)          /* first row of every antidiagonal is 1 */
        return (seq_t)1;
    if (j == 0)          /* first column (except A(0,0)) is 0 */
        return (seq_t)0;

    /* Compute A(i,j) = binomial((j+1)*i + j - 1, i) / (i + 1) */
    seq_t M = (seq_t)(j + 1) * (seq_t)i + j - 1;    /* argument of binomial */
    seq_t bin = binomial((idx_t)M, i);              /* binomial((j+1)*i + j - 1, i) */
    seq_t val = bin / (seq_t)(i + 1);               /* divide by (i+1) */
    return val;
}
