/*
 * A315172  Coordination sequence Gal.5.54.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, for n=0 * a(n) = 4*n + 4*floor(n/9) + 2 - 2*I(n%9==0),
 *            for n>0 * (I(x) is 1 if x is true, 0 otherwise)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t ns = (seq_t)n;
    seq_t res = 4*ns + 4*(ns/9) + 2;
    if (n % 9 == 0) res -= 2;
    return res;
}
