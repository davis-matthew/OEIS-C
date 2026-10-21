/*
 * A286930  Integers whose double is a square and whose triple is a cube.
 * https://oeis.org/A286930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 72*(n-1)^6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 20, u64 n <= 797, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* safe guard, undefined for n<1 */
    seq_t m = (seq_t)n - 1;      /* n-1 as seq_t */
    seq_t m2 = m * m;            /* (n-1)^2 */
    seq_t m3 = m2 * m;           /* (n-1)^3 */
    seq_t m6 = m3 * m3;          /* (n-1)^6 */
    return 72 * m6;              /* 72*(n-1)^6 */
}
