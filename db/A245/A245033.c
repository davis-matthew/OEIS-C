/*
 * A245033  4*(n + 7)^3 - 27*(n + 7)^2 = (4*n +1)*(n+7)^2.
 * https://oeis.org/A245033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n + 7)^3 - 27*(n + 7)^2 = (4*n + 1)*(n + 7)^2
 * Range:     u8 n <= 0, u16 n <= 20, u32 n <= 1019, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m  = (seq_t)n + 7;   /* m = n + 7 */
    seq_t mm = m * m;          /* mm = (n + 7)^2   */
    seq_t mmm = mm * m;        /* mmm = (n + 7)^3  */
    return 4 * mmm - 27 * mm;  /* a(n) */
}
