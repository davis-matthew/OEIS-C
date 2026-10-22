/*
 * A350995  a(n) = (16*10^n - 1)/3.
 * https://oeis.org/A350995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + 3, a(0) = 5
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 5;                /* a(0) */
    for (idx_t i = 0; i < n; ++i) /* iterate n times to reach a(n) */
        val = val * 10 + 3;
    return val;
}
