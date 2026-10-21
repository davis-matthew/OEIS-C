/*
 * A254067  Rectangular array A read by upward antidiagonals in which the
 *          entry in row n and column k is defined by A(n,k) =
 *          S(4*A257499(n,k) - 3), n,k >= 1, where the function S is as
 *          defined in A257480.
 * https://oeis.org/A254067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (3 + 3^r * (6 * k - 3 + 2 * (-1)^r)) / 6, where (r, k)
 *            are the row and column of the A254067 table read by upward
 *            antidiagonals: first element on a diagonal has (r, k) = (d, 1)
 *            and the last has (1, d). d is the diagonal containing index n,
 *            satisfying d(d+1)/2 ≥ n
 * Range:     u8 n <= 6, u16 n <= 37, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal d such that d*(d+1)/2 ≥ n. */
    idx_t d   = 1;
    idx_t sq  = 1;          /* d*(d+1)/2 for current d */
    while (sq < n) {
        ++d;
        sq += d;           /* cum += d */
    }

    /* Position within diagonal (1‑based). */
    idx_t pos = n - (sq - d);   /* sq - d == previous diagonal total */

    /* Convert to (row, column) indices.  The first element on a
       diagonal has (r, k) = (d, 1); the last has (1, d). */
    idx_t r = d - pos + 1;  /* row  (1 ≤ r ≤ d) */
    idx_t k = pos;          /* column (1 ≤ k ≤ d) */

    /* Compute 3^r. */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < r; ++i) {
        pow3 *= (seq_t)3;
    }

    /* partInside = 6*k - 3 + 2*(-1)^r
       For r even: 6*k - 1
       For r odd : 6*k - 5 */
    seq_t partInside;
    if ((r & 1) == 0)
        partInside = (seq_t)6 * (seq_t)k - (seq_t)1;
    else
        partInside = (seq_t)6 * (seq_t)k - (seq_t)5;

    /* Compute numerator = 3 + pow3 * partInside and divide by 6. */
    seq_t numerator = pow3 * partInside + (seq_t)3;
    return numerator / (seq_t)6;
}
