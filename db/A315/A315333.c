/*
 * A315333  Coordination sequence Gal.5.292.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   for n > 0 a(n) = (27*n + d[n mod 10]) / 5, where d = {0, 3, -4,
 *            -1, 2, 0, -2, 1, 4, -3}. a(0) = 1
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return 1;
    static const int d[10] = {0, 3, -4, -1, 2, 0, -2, 1, 4, -3};
    idx_t rem = n % 10u;
    long long num = 27LL * (long long)n + d[rem];
    return (seq_t)(num / 5);
}
