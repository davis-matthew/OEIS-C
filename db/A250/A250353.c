/*
 * A250353  Number of length 4 arrays x(i), i=1..4 with x(i) in i..i+n and no
 *          value appearing more than 2 times.
 * https://oeis.org/A250353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + 4*n^3 + 2*n^2 + 9*n + 1 for n>1; a(1)=16
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)16;
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;      /* n^2 */
    seq_t cbr = sq * sn;     /* n^3 */
    seq_t fourth = sq * sq;  /* n^4 */
    return fourth + (seq_t)4 * cbr + (seq_t)2 * sq + (seq_t)9 * sn + (seq_t)1;
}
