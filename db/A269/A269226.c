/*
 * A269226  Period 6: repeat [3, 9, 6, 6, 9, 3].
 * https://oeis.org/A269226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-6) (period 6) with initial terms [3,9,6,6,9,3]
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {3, 9, 6, 6, 9, 3};
    /* n is 1‑based; (n-1)%6 yields the index 0..5 */
    return pattern[(n - 1) % 6];
}
