/*
 * A313829  Coordination sequence Gal.6.203.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + adjustment * adjustment: * (n == 0) -> +1 * (n ≡ 3
 *            (mod 6)) -> +1 if ((n-3)/6) is even, otherwise -1 * otherwise ->
 *            0
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

    seq_t val = (seq_t)5 * n;

    if ((n % 6) == 3) {
        idx_t k = (n - 3) / 6;
        if ((k & 1) == 0) {   /* even */
            val += 1;
        } else {
            val -= 1;
        }
    }

    return val;
}
