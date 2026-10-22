/*
 * A314672  Coordination sequence Gal.4.38.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + 1 + ( n < 8 ? 0 : (n % 8 == 0 ? 2*(n/8) - 1 :
 *            2*(n/8) ) )
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base linear part */
    seq_t value = (seq_t)n * 4 + 1;

    /* adjustments for n >= 8 */
    idx_t k = n >> 3;                 /* k = n / 8 */
    if (k != 0) {
        idx_t r = n & 7;              /* r = n % 8 */
        seq_t add = 2 * (seq_t)k;     /* 2*(n/8) */
        if (r == 0) add -= 1;         /* subtract 1 when n is a multiple of 8 */
        value += add;                 /* apply the adjustment */
    }

    return value;
}
