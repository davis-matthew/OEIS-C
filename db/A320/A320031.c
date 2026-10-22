/*
 * A320031  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of the e.g.f. exp(x)/(1 - k*x).
 * https://oeis.org/A320031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(i,j), where i and j are obtained from the diagonal
 *            decomposition of the index n, and A(n,k) is defined by: A(0,k) =
 *            1 A(n,k) = 1 + n * k * A(n-1,k) (n > 0) The sequence a(n) is
 *            read by antidiagonals of this square array
 * Range:     u8 n <= 24, u16 n <= 41, u32 n <= 86, u64 n <= 184,
 *            u128 n <= 428, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) {
        ++d;
    }
    idx_t start = d * (d + 1) / 2;     /* first index of diagonal d */
    idx_t offset = n - start;          /* position within the diagonal */
    idx_t i = offset;                  /* row index of A(i,j) */
    idx_t j = d - offset;              /* column index of A(i,j) */

    /* Compute A(i,j) via its recurrence. */
    seq_t val = 1;                     /* A(0,j) */
    for (idx_t m = 1; m <= i; ++m) {
        val = 1 + (seq_t)j * m * val;
    }
    return val;
}
