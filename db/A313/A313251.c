/*
 * A313251  Coordination sequence Gal.6.203.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + δ(n) δ(0) = 1 δ(n) = -1 for n ≡ 1 or 2 (mod 12)
 *            δ(n) = +1 for n ≡ 10 or 11 (mod 12) δ(n) = 0 otherwise
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n * 5;          /* base value 5*n */
    if (n == 0) {
        res += 1;                      /* special case for n = 0 */
    } else {
        idx_t r = n % 12;
        if (r == 1 || r == 2) {
            res -= 1;
        } else if (r == 10 || r == 11) {
            res += 1;
        }
    }
    return res;
}
