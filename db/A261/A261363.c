/*
 * A261363  Triangle read by rows: partial row sums of Sierpinski's triangle.
 * https://oeis.org/A261363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = partial sum of the n-th row of Sierpinski's triangle
 *            (i.e., number of odd binomial coefficients up to the position
 *            corresponding to n). For n = r(r+1)/2 + k with 0 ≤ k ≤ r, a(n) =
 *            #{ j : 0 ≤ j ≤ k and (j & r) == j }
 * Range:     u8 n <= 8000, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r   = 0;            /* current row number          */
    idx_t cum = 0;            /* number of terms before row r */

    while (cum + (r + 1) <= n) {
        cum += (r + 1);
        ++r;
    }

    /* Position k within the row. */
    idx_t k = n - cum;        /* 0 ≤ k ≤ r */

    /* Count j in [0..k] with (j & r) == j (i.e., odd coefficient). */
    seq_t count = 0;
    for (idx_t j = 0; j <= k; ++j) {
        if ((j & r) == j) {
            ++count;
        }
    }
    return count;
}
