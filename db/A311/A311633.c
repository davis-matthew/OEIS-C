/*
 * A311633  Coordination sequence Gal.5.100.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 for n ≥ 1: a(n) = 4*n + 2 * ( number of integers k in
 *            [2,n] with k ≡ 4,5,6 (mod 9) ) This can be expressed as a(n) =
 *            4*n + 2*((n-4)/9 + 1) + 2*((n-5)/9 + 1) + 2*((n-6)/9 + 1) for n
 *            ≥ 1 (empty terms omitted when n < the corresponding bound)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res = ((seq_t)n) * 4;          // base value 4*n

    if (n >= 4) {
        idx_t t = (n - 4) / 9 + 1;
        res += ((seq_t)2) * ((seq_t)t);
    }
    if (n >= 5) {
        idx_t t = (n - 5) / 9 + 1;
        res += ((seq_t)2) * ((seq_t)t);
    }
    if (n >= 6) {
        idx_t t = (n - 6) / 9 + 1;
        res += ((seq_t)2) * ((seq_t)t);
    }
    return res;
}
