/*
 * A380661  Rectangular array neg(i,j,1,2) read by descending antidiagonals:
 *          pos() and neg() denote the positive part and negative part of a
 *          determinant; see Comments.
 * https://oeis.org/A380661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = t * (t + 1) where (i, j) are obtained by reading the
 *            array neg(i, j, 1, 2) in ascending antidiagonals. For each
 *            anti‑diagonal d = i + j, the elements are (1, d-1), (2, d-2), …
 *            , (d-1, 1). With i and j determined, t = (j² + (2i‑1)j + i(i+1))
 *            / 2
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1; below offset is undefined */

    /* find the diagonal d where the nth element lies.
       (d-1)*d/2   is the number of elements up to diagonal d.   */
    idx_t d = 2;
    while ((d - 1) * d / 2 < n) d++;

    /* previous number of elements before this diagonal */
    idx_t prev = (d - 2) * (d - 1) / 2;

    /* offset within the diagonal (0‑based) */
    idx_t k = n - prev - 1;

    /* coordinates i and j of the required entry */
    idx_t i = k + 1;
    idx_t j = (d - 1) - k;

    /* compute t = (j² + (2i‑1)j + i(i+1)) / 2  */
    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;
    seq_t num = jj * jj + ((seq_t)2 * ii - 1) * jj + ii * (ii + 1);
    seq_t t = num / 2;

    /* return the product t*(t+1) which equals neg(i,j,1,2) */
    return t * (t + 1);
}
