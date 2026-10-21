/*
 * A273367  Numbers k such that 10*k+6 is a perfect square.
 * https://oeis.org/A273367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = 10*n^2 + 8*n + 1; a(2n+1) = 10*n^2 + 12*n + 3
 * Range:     u8 n <= 9, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0ULL) {
        /* even n = 2m */
        idx_t m = n >> 1;          /* m = n/2 */
        seq_t mm = (seq_t)m;
        return (seq_t)10 * mm * mm + (seq_t)8 * mm + 1;
    } else {
        /* odd n = 2m+1 */
        idx_t m = n >> 1;          /* m = floor(n/2) */
        seq_t mm = (seq_t)m;
        return (seq_t)10 * mm * mm + (seq_t)12 * mm + 3;
    }
}
