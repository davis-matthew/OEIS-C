/*
 * A336535  a(n) = (m(n)^2 + 3)*(m(n)^2 + 7)/32, where m(n) = 2*n - 1.
 * https://oeis.org/A336535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - n + 1)*(n^2 - n + 2)/2 Since n^2 - n is even, (n^2
 *            - n + 2)/2 is an integer. Compute as (n^2 - n + 1) * ((n^2 - n +
 *            2)/2) to avoid overflow
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_sq = (seq_t)n * (seq_t)n;        /* n^2 */
    seq_t t = n_sq - (seq_t)n + 1;           /* n^2 - n + 1 */
    seq_t t2_half = (t + 1) >> 1;            /* (n^2 - n + 2)/2 */
    return t * t2_half;
}
