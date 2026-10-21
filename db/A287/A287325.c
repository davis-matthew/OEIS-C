/*
 * A287325  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of Sum_{j=-inf..inf}
 *          (-1)^j*x^(k*j*(j-1)/2 + j^2).
 * https://oeis.org/A287325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 807, i16 n <= 5049, i32 n <= 5049, i64 n <= 5049,
 *            i128 n <= 5049, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A287325
   a(n) is obtained by reading the square array A(n,k) along
   antidiagonals.  For a fixed column k, the generating function is

        S_k(x) = Σ_{j∈Z} (-1)^j x^{k*j*(j-1)/2 + j^2}.

   The entry a(n) equals the coefficient of x^row in the
   expansion of column col, where (row,col) are obtained from
   n by the usual antidiagonal mapping:

        let d be the minimal integer with (d+1)(d+2)/2 > n
        let p = d*(d+1)/2   (number of elements before diag d)
        let offset = n - p
        then row = offset
             col = d - offset
   The coefficient is computed by summing (-1)^j over all
   integers j with k*j*(j-1)/2 + j^2 == row.  This algorithm
   runs in O(√n) time and uses only integer arithmetic.  */

#include <stdint.h>
#include <limits.h>

seq_t a(idx_t n)
{
    /* Find the diagonal d such that n lies in it.  */
    idx_t low = 0, high = n;
    /* Binary search for minimal d with (d+1)(d+2)/2 > n.  */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        /* mid*(mid+1)/2 gives the count of elements before diag mid. */
        if ((mid + 1) * (mid + 2) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t d = low;                      /* diagonal index */

    /* Compute row and column indices.  */
    idx_t prev = d * (d + 1) / 2;        /* elements before diag d */
    idx_t offset = n - prev;             /* position inside the diag */
    idx_t row = offset;                  /* row index (0‑based)   */
    idx_t col = d - offset;              /* column index (0‑based) */

    /* Compute the coefficient by direct summation over all j.  */
    seq_t result = 0;
    /* Since row <= d <= sqrt(2n)+1, row is modest; loop from -row..row. */
    for (int64_t j = -(int64_t)row; j <= (int64_t)row; ++j) {
        seq_t exp = (seq_t)col * (seq_t)j * (seq_t)(j - 1) / 2
                    + (seq_t)j * (seq_t)j;
        if (exp == (seq_t)row) {
            result += ((j & 1) ? -1 : 1);
        }
    }
    return result;
}
