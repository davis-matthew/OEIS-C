/*
 * A320592  Number of partitions of n with four parts in which no part occurs
 *          more than twice.
 * https://oeis.org/A320592
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+3*n^2-48*n+144)/144 - (n mod 2)*n/16)
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 1835, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 6) return 0;           /* behaviour for n < 6 is irrelevant        */
    seq_t nn = n;
    seq_t temp = nn*nn*nn            /* n^3      */
              + 3*nn*nn              /* 3*n^2    */
              - 48*nn;               /* -48*n    */
    temp += 144;                    /* +144     */
    if (nn & 1ULL)
        temp -= 9*nn;               /* subtract 9*n when n is odd               */
    return temp / 144;
}
