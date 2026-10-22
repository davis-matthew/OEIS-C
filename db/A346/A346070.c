/*
 * A346070  Symbolic code for the corner turns in the Lévy dragon curve.
 * https://oeis.org/A346070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A007814(n) mod 4 (number of trailing zero bits of n,
 *            modulo 4)
 * Range:     u8 n <= 9903, u16 n <= 9903, u32 n <= 9903, u64 n <= 9903,
 *            u128 n <= 9903, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by the sequence offset */
    idx_t cnt = 0;
    idx_t m = n;
    while ((m & 1ULL) == 0ULL) {
        ++cnt;
        m >>= 1;
    }
    return (seq_t)(cnt & 3ULL); /* cnt % 4 */
}
