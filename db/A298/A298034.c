/*
 * A298034  Partial sums of A298033.
 * https://oeis.org/A298034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^2 + 2)/2 for n even, * a(n) = (9*n^2 + 5)/2 for n
 *            odd
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nsq = (seq_t)n * (seq_t)n;     /* n^2 */
    if ((n & 1) == 0)                   /* even n */
        return ((seq_t)9 * nsq + 2) / 2;
    else                                 /* odd n */
        return ((seq_t)9 * nsq + 5) / 2;
}
