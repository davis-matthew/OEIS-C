/*
 * A312304  Coordination sequence Gal.3.17.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1 * a(1) = 4 * For n odd and n > 1: a(n) = 4*n + 2 *
 *            For n even, let m = n/2, k = floor(m/4), r = m % 4 * If r == 0:
 *            a(n) = 4*n + (4*k - 2) * If r == 1: a(n) = 4*n + (8*k) * If r ==
 *            2: a(n) = 4*n + (4*k) * If r == 3: a(n) = 4*n + (8*k + 4) * *
 *            This piecewise linear description reproduces...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle special cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* Odd indices ≥ 3 */
    if (n & (idx_t)1) /* n is odd */
        return ((seq_t)n * (seq_t)4) + (seq_t)2;

    /* Even indices: compute using the derived formula */
    idx_t m = n >> 1;            /* m = n/2 */
    idx_t k = m >> 2;            /* k = floor(m/4) */
    idx_t r = m & (idx_t)3;      /* r = m % 4 */

    seq_t e;                     /* correction term */

    if (r == (idx_t)0) {
        /* m is a multiple of 4 (m ≥ 4 since n ≠ 0) */
        e = (seq_t)(4 * k - 2);
    } else if (r == (idx_t)1) {
        e = (seq_t)(8 * k);
    } else if (r == (idx_t)2) {
        e = (seq_t)(4 * k);
    } else /* r == 3 */ {
        e = (seq_t)(8 * k + 4);
    }

    return ((seq_t)n * (seq_t)4) + e;
}
