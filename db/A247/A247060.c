/*
 * A247060  Dynamic Betting Game D(n,4,1).
 * https://oeis.org/A247060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(8*n/5) + 2*floor(4*n/5)
 * Range:     u8 n <= 79, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t t1 = (nn * (seq_t)8) / (seq_t)5;
    seq_t t2 = (nn * (seq_t)4) / (seq_t)5;
    return t1 + (seq_t)2 * t2;
}
