/*
 * A315260  Coordination sequence Gal.6.340.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + c(n) where c(n) is periodic with period 12: c(0) =
 *            1, and for n ≥ 1, c(n) = 1 if n ≡ 1 (mod 12), -1 if n ≡ 11 (mod
 *            12), 0 otherwise
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* periodic correction array for n ≥ 1 */
    static const int pat[12] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0 };

    idx_t r = (n - 1) % 12;               /* 0 … 11 */
    long long tmp = 5 * (long long)n + pat[r];
    return (seq_t)tmp;
}
