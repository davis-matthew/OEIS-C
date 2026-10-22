/*
 * A396280  a(1) = 2, a(2) = 8, a(3) = 32; a(n) = 16*(n-2)^3 for n >= 4.
 * https://oeis.org/A396280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2, 8, 32 for n = 1, 2, 3 a(n) = 16*(n-2)^3 for n >= 4
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 647, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)32;
    seq_t m = (seq_t)(n - 2);
    /* compute 16 * m * m * m */
    return (seq_t)16 * m * m * m;
}
