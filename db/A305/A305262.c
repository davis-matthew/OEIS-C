/*
 * A305262  a(n) = 140*2^n - 127.
 * https://oeis.org/A305262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 140*2^n - 127, with offset 0 (a(0)=13)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;                 /* 2^0 */
    for (idx_t i = 0; i < n; ++i) {
        val <<= 1;                 /* multiply by 2 */
    }
    val = 140 * val - 127;         /* 140*2^n - 127 */
    return val;
}
