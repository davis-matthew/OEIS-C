/*
 * A315729  Coordination sequence Gal.5.134.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n - 4*floor(n/8) - 2*extra where extra = 0 if n mod 8 <
 *            4, 1 if n mod 8 == 4, 2 if n mod 8 >= 5 a(0) = 1. This
 *            implements OEIS sequence A315729
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;        /* a(0) = 1 */

    idx_t blocks = n / 8;          /* number of full 8‑blocks */
    idx_t rem    = n % 8;          /* remainder in the last block */

    /* extra counts how many of {4,5} are present in the incomplete block */
    seq_t extra = 0;
    if (rem >= 4) extra++;         /* 4 */
    if (rem >= 5) extra++;         /* 5 */

    /* compute a(n) = 6*n - 4*blocks - 2*extra */
    seq_t n_s = (seq_t)n;
    seq_t res = 6 * n_s;            /* 6*n */
    res -= (seq_t)4 * (seq_t)blocks;
    res -= (seq_t)2 * extra;
    return res;
}
