/*
 * A282180  a(n+1) = a(n)*(a(n)^2 - 3) with a(0) = 8.
 * https://oeis.org/A282180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A282180: a(0) = 8, a(n+1) = a(n)*(a(n)^2 - 3) */
seq_t a(idx_t n)
{
    seq_t val = 8;
    for (idx_t i = 0; i < n; ++i) {
        seq_t tmp = val * val - 3;
        val = val * tmp;
    }
    return val;
}
