/*
 * A295388  a(n) is the least k > n such that n divides k, and n+1 divides
 *          k+1, and n+2 divides k+2.
 * https://oeis.org/A295388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n-1) = 8*n^3 - 1, a(2n) = 4*n^3 + 6*n^2 + 4*n
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1) {
        idx_t m = (n + 1) / 2;          /* m = (n+1)/2 for odd n */
        seq_t mm = (seq_t)m;
        return (seq_t)8 * mm * mm * mm - (seq_t)1;
    } else {
        idx_t m = n / 2;                 /* m = n/2 for even n */
        seq_t mm = (seq_t)m;
        return (seq_t)4 * mm * mm * mm + (seq_t)6 * mm * mm + (seq_t)4 * mm;
    }
}
