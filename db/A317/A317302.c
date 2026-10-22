/*
 * A317302  Square array T(n,k) = (n - 2)*(k - 1)*k/2 + k, with n >= 0, k >=
 *          0, read by antidiagonals upwards.
 * https://oeis.org/A317302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) is the n‑th term of square array T(n,k)=((n-2)*(k-1)*k)/2 +
 *            k read by anti‑diagonals upwards
 * Range:     i8 n <= 84, i16 n <= 84, i32 n <= 84, i64 n <= 84, i128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find the anti‑diagonal d such that index n lies on it.
       For each d, there are d+1 entries.  The cumulative count
       of entries before diagonal d is d*(d+1)/2.  The smallest
       d with (d+1)*(d+2)/2 > n gives the desired diagonal. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) {
        d++;
    }
    /* Number of entries preceding this diagonal */
    idx_t cum_prev = d * (d + 1) / 2;
    /* Position within the diagonal */
    idx_t offset = n - cum_prev;
    /* Coordinates (i,j) of the element: i decreases, j increases. */
    idx_t i = d - offset;
    idx_t j = offset;
    /* Compute the table entry.  All arithmetic is in seq_t. */
    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;
    seq_t val = ((ii - 2) * (jj - 1) * jj) / 2 + jj;
    return val;
}
