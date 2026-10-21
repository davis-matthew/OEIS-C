/*
 * A241577  a(n) = n^3 + 4*n^2 - 5*n + 1.
 * https://oeis.org/A241577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + 4*n^2 - 5*n + 1
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t nsq  = nn * nn;
    seq_t ncub = nsq * nn;
    return ncub + (seq_t)4 * nsq - (seq_t)5 * nn + (seq_t)1;
}
