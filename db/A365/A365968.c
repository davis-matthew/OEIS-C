/*
 * A365968  Irregular triangle read by rows: T(n,k) (0 <= n, 0 <= k < 2^n). An
 *          infinite binary tree with root node 0 in row n = 0. Each node then
 *          has left child (2*j) - k - 1 and right child (2*j) - k + 1, where
 *          j and k are the values of the parent and grandparent nodes
 *          respectively.
 * https://oeis.org/A365968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, look, easy
 * Formula:   T(n,k) = - Sum_{i=0..n-1} (i+1)*(-1)^{b[i]} where k = Σ b[i]*2^i
 * Range:     i8 n <= 8190, i16 n <= 8190, i32 n <= 8190, i64 n <= 8190,
 *            i128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) enumerates T(row,col) by rows: row = floor(log2(n+1)), col = n-(2^row-1) */

seq_t a(idx_t n)
{
    /* Determine the row containing index n */
    idx_t row = 63 - __builtin_clzll(n + 1ULL);          /* floor(log2(n+1)) */
    idx_t pow2 = ((idx_t)1) << row;                    /* 2^row */
    idx_t start = pow2 - 1;                            /* first index of this row */
    idx_t col = n - start;                             /* column within the row */

    seq_t sum = 0;
    for (idx_t i = 0; i < row; ++i)
    {
        uint64_t bit = (col >> i) & 1ULL;               /* bit b[i] of k */
        seq_t term = (seq_t)(i + 1) * (bit ? -((seq_t)1) : ((seq_t)1));
        sum += term;
    }
    return -sum;
}
