/*
 * A389177  Number of mutual-visibility sets in the n-cycle graph.
 * https://oeis.org/A389177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n*(7 + 30*n + 2*n^2 + 3*(-1)^n*(2*n - 5))/48.
 *            Equivalent form used here: if n is odd: a = n*(n+11)*(n+1)/24 +
 *            1 if n is even: a = n*(n^2 + 18*n - 4)/24 + 1
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    if (n & 1) {            /* n is odd */
        return (n_t * (n_t + 11) * (n_t + 1)) / 24 + 1;
    } else {                /* n is even */
        return (n_t * (n_t * n_t + 18 * n_t - 4)) / 24 + 1;
    }
}
