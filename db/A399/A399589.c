/*
 * A399589  Product of the distinct entries in the central row and column of
 *          the (2*n + 1) X (2*n + 1) square array formed by the integers 1,
 *          2, ..., (2*n + 1)^2 in order.
 * https://oeis.org/A399589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = product of the distinct entries in the central row and
 *            column of the (2*n + 1) × (2*n + 1) square array formed by the
 *            integers 1, 2, …, (2*n + 1)^2 in row‑major order
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t s = 2 * n + 1;          /* side length of the array */
    seq_t product = 1;

    /* Multiply the entries of the central row.  Row index = n. */
    seq_t row_start = (seq_t)n * (seq_t)s + 1;   /* first number in row n */
    for (idx_t col = 0; col < s; ++col) {
        seq_t val = row_start + (seq_t)col;      /* number at (n, col) */
        product *= val;
    }

    /* Multiply the entries of the central column, skipping the already
       counted center element. */
    for (idx_t r = 0; r < s; ++r) {
        if (r == n) continue;                    /* center already counted */
        seq_t val = (seq_t)r * (seq_t)s + (seq_t)n + 1; /* number at (r, n) */
        product *= val;
    }

    return product;
}
