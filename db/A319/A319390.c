/*
 * A319390  a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) - a(n-4) + a(n-5), a(0)=1,
 *          a(1)=2, a(2)=3, a(3)=6, a(4)=8.
 * https://oeis.org/A319390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = (3*n^2 + n + 2)/2, a(2n+1) = (3*n^2 + 5*n + 4)/2
 * Range:     u8 n <= 25, u16 n <= 417, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor(n/2) as seq_t. */
    seq_t m = (seq_t)(n / 2);           /* m = ⌊n/2⌋ */

    if ((n & 1) == 0) {                 /* n even */
        return ( (seq_t)3 * m * m + m + (seq_t)2 ) / 2;
    } else {                            /* n odd */
        return ( (seq_t)3 * m * m + (seq_t)5 * m + (seq_t)4 ) / 2;
    }
}
