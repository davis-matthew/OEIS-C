/*
 * A288919  Number of 5-cycles in the n X n king graph.
 * https://oeis.org/A288919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*n^2 - 128*n + 112 for n > 1; a(1)=a(2)=0
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)0;
    seq_t nt = (seq_t)n;
    seq_t res = (seq_t)36 * nt * nt - (seq_t)128 * nt + (seq_t)112;
    return res;
}
