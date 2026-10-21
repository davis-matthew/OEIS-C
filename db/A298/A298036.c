/*
 * A298036  Coordination sequence of Dual(4.6.12) tiling with respect to a
 *          12-valent node.
 * https://oeis.org/A298036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = 6*n if n even, otherwise 12*n
 * Range:     u8 n <= 22, u16 n <= 5462, u32 n <= 9279, u64 n <= 9279,
 *            u128 n <= 9279, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t n_seq = (seq_t)n;
    if (n & 1)  /* n is odd */
        return n_seq * (seq_t)12;
    else        /* n is even */
        return n_seq * (seq_t)6;
}
