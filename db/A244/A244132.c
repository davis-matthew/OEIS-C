/*
 * A244132  Triangle read by rows: coefficients T(n,k) of a binomial
 *          decomposition of n as Sum(k=0..n)T(n,k)*binomial(n,k).
 * https://oeis.org/A244132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(row, col) where T(r,0) = 0, T(r,k) = k^(k-1) *
 *            (1-k)^(r-k) for 1 ≤ k ≤ r. row, col are determined by n in the
 *            triangular indexing: row = smallest r with (r+1)(r+2)/2 > n; col
 *            = n - r*(r+1)/2
 * Range:     i8 n <= 18, i16 n <= 33, i32 n <= 74, i64 n <= 168,
 *            i128 n <= 432, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    /* find the row containing the n‑th element */
    idx_t row = 0;
    idx_t cumulative = 1;   /* (row+1)(row+2)/2 for current row */
    while (cumulative <= n) {
        ++row;
        cumulative = (row + 1) * (row + 2) / 2;
    }
    idx_t prev_cumulative = row * (row + 1) / 2;
    idx_t col = n - prev_cumulative;

    if (col == 0) return (seq_t)0;          /* T(r,0) = 0 */

    /* compute T(row,col) = col^(col-1) * (1-col)^(row-col) */
    seq_t pow1 = pow_seq_t((seq_t)col, col - 1);   /* col^(col-1) */
    seq_t base2 = -(seq_t)(col - 1);              /* 1 - col = -(col-1) */
    seq_t pow2 = pow_seq_t(base2, row - col);      /* (1-col)^(row-col) */

    return pow1 * pow2;
}
