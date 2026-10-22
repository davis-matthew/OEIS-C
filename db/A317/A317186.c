/*
 * A317186  One of many square spiral sequences: a(n) = n^2 + n -
 *          floor((n-1)/2).
 * https://oeis.org/A317186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + n - floor((n-1)/2)
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)                 /* a(0) = 1 */
        return (seq_t)1;
    seq_t nn   = (seq_t)n;      /* cast index to seq_t */
    seq_t sq   = nn * nn;       /* n^2 */
    seq_t half = (nn - (seq_t)1) / (seq_t)2; /* floor((n-1)/2) */
    return sq + nn - half;      /* n^2 + n - floor((n-1)/2) */
}
