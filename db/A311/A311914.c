/*
 * A311914  Coordination sequence Gal.4.58.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, for n = 0 * a(n) = 4*n + c(n), for n ≥ 1 * c(n) = #{ k
 *            ∈ [2, n] | k mod 7 = 3 or 5 } * Thus a(n) = 4*n + ⌊(n-3)/7⌋ + 1
 *            (if n ≥ 3) + ⌊(n-5)/7⌋ + 1 (if n ≥ 5)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }

    seq_t nn = (seq_t)n;
    seq_t count = 0;

    if (n >= 3) {
        count += ( (n - 3) / 7 ) + 1;
    }
    if (n >= 5) {
        count += ( (n - 5) / 7 ) + 1;
    }

    return nn * 4 + count;
}
