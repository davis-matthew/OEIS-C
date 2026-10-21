/*
 * A272129  a(n) = 32*n^2 - 56*n + 25.
 * https://oeis.org/A272129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*n^2 - 56*n + 25
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n_sq  = n_seq * n_seq;        /* n^2 */
    seq_t result = 32 * n_sq + (seq_t)25; /* 32*n^2 + 25 */
    result -= 56 * n_seq;               /* subtract 56*n */
    return result;
}
