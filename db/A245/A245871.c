/*
 * A245871  Number of length 2+2 0..n arrays with some pair in every
 *          consecutive three terms totalling exactly n.
 * https://oeis.org/A245871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 4*n + 7*n^2 + n^3 (even n) a(n) = -2 + 4*n + 7*n^2 +
 *            n^3 (odd n)
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n_sq = nn * nn;
    seq_t n_cu = n_sq * nn;     /* n^3 */
    seq_t part2 = 7 * n_sq;
    seq_t part3 = 4 * nn;

    if ((n & 1) == 0)           /* even */
        return 1 + part3 + part2 + n_cu;
    else                        /* odd */
        return part3 + part2 + n_cu - 2;
}
