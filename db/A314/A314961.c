/*
 * A314961  Coordination sequence Gal.5.137.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + d(n), where d(n) is periodic with period 10: d(n) =
 *            0, -1, -1, 0, 0, 0, 1, 1, 0, 0 for n ≡ 1,…,0 (mod 10). a(0) = 1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t nn = (seq_t)n;              /* base term 5*n */
    static const int off[10] = {0, 0, -1, -1, 0, 0, 0, 1, 1, 0};
    int o = off[n % 10];
    seq_t result = nn * 5;
    if (o == -1)
        result -= 1;
    else if (o == 1)
        result += 1;
    return result;
}
