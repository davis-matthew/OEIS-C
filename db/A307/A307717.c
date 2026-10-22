/*
 * A307717  Number of palindromic squares, k^2, of length n such that k is
 *          also palindromic.
 * https://oeis.org/A307717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=4; a(2*n)=0; a(4*m+1)=(m^3-3*m^2+11*m+6)/3;
 *            a(4*m+3)=(m^3+5*m+12)/6
 * Range:     u8 n <= 26, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)4;
    if ((n & (idx_t)1) == 0) return 0;
    if ((n % 4) == 1) {
        seq_t m = (seq_t)((n - 1) / 4);
        seq_t m2 = m * m;
        seq_t m3 = m2 * m;
        seq_t t = m3 - (seq_t)3 * m2 + (seq_t)11 * m + (seq_t)6;
        return t / (seq_t)3;
    } else { /* n % 4 == 3 */
        seq_t m = (seq_t)((n - 3) / 4);
        seq_t m2 = m * m;
        seq_t m3 = m2 * m;
        seq_t t = m3 + (seq_t)5 * m + (seq_t)12;
        return t / (seq_t)6;
    }
}
