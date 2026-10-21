/*
 * A251754  Digital root of A027444(n) = n + n^2 + n^3, n>=1. Repeat(3, 5, 3,
 *          3, 2, 6, 3, 8, 9).
 * https://oeis.org/A251754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = digital_root(n + n^2 + n^3)
 * Range:     u8 n <= 6, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1, 0 is undefined */
        return 0;

    /* Work modulo 9: digital_root(x) = 9 if x ≡ 0 (mod 9), else x mod 9. */
    seq_t nm = (seq_t)n % 9;          /* n mod 9 */
    seq_t sq = nm * nm;               /* n^2 mod 9 */
    seq_t cb = sq * nm;               /* n^3 mod 9 */
    seq_t s  = (nm + sq + cb) % 9;    /* (n + n^2 + n^3) mod 9 */

    return (s == 0) ? 9 : s;          /* convert 0→9 for digital root */
}
