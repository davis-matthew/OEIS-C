/*
 * A315344  Coordination sequence Gal.6.640.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 6 For n >= 2: let q = (n - 2) / 10 r = (n - 2) %
 *            10 if r < 7: a(n) = 6*n - 2*(1 + 2*q) else: a(n) = 6*n - 4*(1 +
 *            q)
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 6;

    /* n >= 2 */
    idx_t q = (n - 2) / 10;
    idx_t r = (n - 2) % 10;

    seq_t base = (seq_t)n * 6;

    seq_t sub;
    if (r < 7) {
        /* 2*(1 + 2*q) */
        sub = (seq_t)2 * ((seq_t)1 + (seq_t)2 * (seq_t)q);
    } else {
        /* 4*(1 + q) */
        sub = (seq_t)4 * ((seq_t)1 + (seq_t)q);
    }
    return base - sub;
}
