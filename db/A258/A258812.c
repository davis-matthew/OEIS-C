/*
 * A258812  a(n) = n^11 - 1.
 * https://oeis.org/A258812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^11 - 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 0, a(2) = 2047, ... computed as n^11 - 1 */
    seq_t result = 1;
    for (idx_t i = 0; i < 11; ++i)
        result *= (seq_t)n;
    return result - 1;
}
