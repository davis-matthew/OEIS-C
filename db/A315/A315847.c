/*
 * A315847  Coordination sequence Gal.5.232.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = 6*n if n % 3 == 0 a(n) = (13*n +
 *            5)/3 if n % 3 == 1 a(n) = (13*n - 5)/3 if n % 3 == 2
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)1;
    }

    idx_t r = n % 3u;
    if (r == 0u) {
        return (seq_t)(6u * n);
    } else if (r == 1u) {
        return (seq_t)((13u * n + 5u) / 3u);
    } else {                /* r == 2 */
        return (seq_t)((13u * n - 5u) / 3u);
    }
}
