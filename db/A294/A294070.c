/*
 * A294070  a(n) = (1/4)*(n^2 - 2*n)^2 + (9/4)*(n^2 - 2*n) + 6.
 * https://oeis.org/A294070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - 3*n + 6)*(n^2 - n + 4)/4
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 362, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* defined only for n ≥ 1 */
    /* Work in seq_t to avoid overflow, using explicit casts if needed */
    seq_t nn = (seq_t)n;           /* n as seq_t */
    seq_t m  = nn * (nn - 2);      /* m = n*(n-2) = n^2 - 2n */
    seq_t num = m * m + 9 * m + 24;/* numerator: m^2 + 9m + 24  */
    return num / 4;                /* division is exact */
}
