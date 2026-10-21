/*
 * A295340  Numbers congruent to 11 or 13 mod 15.
 * https://oeis.org/A295340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (15*n - 4)/2 for n even; a(n) = (15*n + 7)/2 for n odd
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * 15;
    if ((n & 1) == 0) {
        return (t - 4) / 2;
    } else {
        return (t + 7) / 2;
    }
}
