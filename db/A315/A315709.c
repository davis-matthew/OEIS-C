/*
 * A315709  Coordination sequence Gal.6.627.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n - k where k is the block index determined by offset =
 *            n - 3 q = offset / 11 (integer division) r = offset - 11*q
 *            (remainder) k = (r < 6) ? 2*q + 1 : 2*q + 2 For n = 0,1,2 the
 *            values are 1, 6, 12 respectively
 * Range:     u8 n <= 43, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)12;

    seq_t nn = (seq_t)n;
    seq_t base = (seq_t)6 * nn;          /* 6*n */

    idx_t offset = n - 3;
    idx_t q = offset / 11;
    idx_t r = offset - q * 11;           /* another way to get offset % 11 */
    idx_t k = (r < 6) ? (2 * q + 1) : (2 * q + 2);
    seq_t diff = (seq_t)k;

    return base - diff;
}
