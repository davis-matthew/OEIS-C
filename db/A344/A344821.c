/*
 * A344821  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals
 *          downwards, where A(n,k) = Sum_{j=1..n} floor(n/j) * k^(j-1).
 * https://oeis.org/A344821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th term when the square array A(n,k)=∑_{j=1}^{n}
 *            ⌊n/j⌋·k^{j−1} is read by antidiagonals downwards (first column
 *            first, each diagonal in increasing n)
 * Range:     u8 n <= 40, u16 n <= 85, u32 n <= 201, u64 n <= 483,
 *            u128 n <= 1208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find diagonal s such that (s+1)(s+2)/2 >= n */
    idx_t s = 0;
    while ( ((s + 1) * (s + 2)) / 2 < n ) {
        ++s;
    }
    /* Number of terms before this diagonal */
    idx_t prev = (s * (s + 1)) / 2;
    /* Position inside the diagonal (1‑based) */
    idx_t offset = n - prev;
    /* Corresponding coordinates in the array */
    idx_t row = offset;           /* n in A(n,k) */
    idx_t col = s - offset + 1;   /* k in A(n,k) */

    /* Compute A(row, col) */
    seq_t sum = 0;
    seq_t pow = 1; /* col^{j-1} */
    for (idx_t j = 1; j <= row; ++j) {
        seq_t f = (seq_t)(row / j); /* floor(row/j) */
        sum += f * pow;
        pow *= (seq_t)col;
    }
    return sum;
}
