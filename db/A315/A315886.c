/*
 * A315886  Coordination sequence Gal.6.527.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(1) = 6; for n >= 2, let cycle = (n-2)/7, offset =
 *            (n-2)%7 (0 <= offset <= 6) r = 0 if offset < 2 1 if offset < 4 2
 *            otherwise then m = 3*cycle + r + 1 a(n) = 6*n - 4*m
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 6;

    /* n >= 2 */
    idx_t cycle  = (n - 2) / 7;
    idx_t offset = (n - 2) % 7;

    idx_t r;
    if (offset < 2)
        r = 0;
    else if (offset < 4)
        r = 1;
    else
        r = 2;

    idx_t m = 3 * cycle + r + 1;

    return (seq_t)6 * (seq_t)n - (seq_t)4 * (seq_t)m;
}
