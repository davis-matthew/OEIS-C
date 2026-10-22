/*
 * A310378  Coordination sequence Gal.6.320.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12, a(0) = 1, a(1) =
 *            4, a(2) = 10, a(3) = 14, a(4) = 18, a(5) = 24, a(6) = 28, a(7) =
 *            34, a(8) = 38, a(9) = 42, a(10) = 48, a(11) = 52, a(12) = 56
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values a(0) through a(12) */
    static const seq_t init[13] = {
        1,  4, 10, 14, 18, 24, 28, 34, 38, 42, 48, 52, 56
    };

    if (n < 13)
        return init[n];

    /* Circular buffer holding a(i-12) … a(i-1) for current i */
    seq_t buf[13];
    for (idx_t k = 0; k < 13; ++k)
        buf[k] = init[k];

    for (idx_t i = 13; i <= n; ++i) {
        idx_t idx  = i % 13;          /* position to store a(i)   */
        idx_t i1   = (i - 1) % 13;    /* a(i-1)                   */
        idx_t i11  = (i - 11) % 13;   /* a(i-11)                  */
        idx_t i12  = (i - 12) % 13;   /* a(i-12)                  */
        buf[idx] = buf[i1] + buf[i11] - buf[i12];
    }

    return buf[n % 13];
}
