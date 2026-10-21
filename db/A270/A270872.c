/*
 * A270872  a(n) = n^8 + 7*n^7 + 34*n^6 + 111*n^5 + 275*n^4 + 511*n^3 +
 *          703*n^2 + 623*n + 13.
 * https://oeis.org/A270872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8 + 7*n^7 + 34*n^6 + 111*n^5 + 275*n^4 + 511*n^3 +
 *            703*n^2 + 623*n + 13
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 15, u64 n <= 255, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x   = (seq_t)n;
    seq_t x2  = x * x;            // x^2
    seq_t x3  = x2 * x;           // x^3
    seq_t x4  = x3 * x;           // x^4
    seq_t x5  = x4 * x;           // x^5
    seq_t x6  = x5 * x;           // x^6
    seq_t x7  = x6 * x;           // x^7
    seq_t x8  = x7 * x;           // x^8
    return x8
           + (seq_t)7   * x7
           + (seq_t)34  * x6
           + (seq_t)111 * x5
           + (seq_t)275 * x4
           + (seq_t)511 * x3
           + (seq_t)703 * x2
           + (seq_t)623 * x
           + (seq_t)13;
}
