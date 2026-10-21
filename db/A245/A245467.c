/*
 * A245467  a(n) = ( 4*n^2 - 2*n + 1 - (2*n^2 - 6*n + 1) * (-1)^n )/16.
 * https://oeis.org/A245467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n%2==0) ? (n/2)*(n/2+1)/2 : (n/2)*(3*(n/2)+1)/2
 * Range:     u8 n <= 26, u16 n <= 418, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = n >> 1;          /* floor(n/2) */
    if ((n & 1) == 0) {        /* even n */
        return m * (m + 1) / 2;
    } else {                   /* odd n */
        return m * (3 * m + 1) / 2;
    }
}
