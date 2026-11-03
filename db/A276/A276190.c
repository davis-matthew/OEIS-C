/*
 * A276190  Sum of the squares of the digits of the base-4 representation of
 *          n.
 * https://oeis.org/A276190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of squares of the digits of n in base-4
 *            representation
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 80 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t m = n;
    while (m) {
        seq_t d = m & 3;   /* least significant base-4 digit */
        sum += d * d;
        m >>= 2;           /* move to next base-4 digit */
    }
    return sum;
}
