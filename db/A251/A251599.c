/*
 * A251599  Centers of rows of the triangular array formed by the natural
 *          numbers.
 * https://oeis.org/A251599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*m^2+2*m+1 if n%3==1; 2*m^2+4*m+2 if n%3==2; 2*m^2+4*m+3
 *            if n%3==0; where m = floor((n-1)/3)
 * Range:     u8 n <= 33, u16 n <= 543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = floor((n-1)/3) */
    idx_t m = (n - 1) / 3;
    idx_t r = n % 3;            /* n modulo 3 */

    /* t = m*m */
    seq_t t = (seq_t)m * (seq_t)m;

    switch (r) {
        case 1:
            return (seq_t)2 * t + (seq_t)2 * (seq_t)m + (seq_t)1;
        case 2:
            return (seq_t)2 * t + (seq_t)4 * (seq_t)m + (seq_t)2;
        case 0:
        default:
            return (seq_t)2 * t + (seq_t)4 * (seq_t)m + (seq_t)3;
    }
}
