/*
 * A297996  a(1)=2, a(2)=3, a(3)=5 and a(n) = (a(1) + a(2) + a(3) + ... +
 *          a(n-1))/a(n-1).
 * https://oeis.org/A297996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = n/2 if n even, a(n) = (n+7)/2 if n odd; a(1)=2, a(2)=3.
 *            (A297996)
 * Range:     u8 n <= 297, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;

    seq_t sn = (seq_t)n;
    /* use the parity of the original index */
    if ((n & 1ULL) == 0)          /* even n */
        return sn / 2;
    else                          /* odd n */
        return (sn + (seq_t)7) / 2;
}
