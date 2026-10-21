/*
 * A260955  Differences of the increasing arithmetic progression a^2+a, b^2+b,
 *          c^2+c, where b = 5*a+2, c = 7*a+3 and a >= 0.
 * https://oeis.org/A260955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*n^2 + 24*n + 6
 * Range:     u8 n <= 2, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq   = (seq_t)n;
    seq_t prod    = n_seq * (n_seq + (seq_t)1);          /* n*(n+1) */
    seq_t result  = (seq_t)24 * prod + (seq_t)6;         /* 24*n*(n+1)+6 */
    return result;
}
