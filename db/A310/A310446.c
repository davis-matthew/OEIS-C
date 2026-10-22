/*
 * A310446  Coordination sequence Gal.5.137.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + Δ(n) Δ(0) = 1 For n > 0: Δ(n) = -1 if n ≡ 1 or 4
 *            (mod 10) +1 if n ≡ 6 or 9 (mod 10) 0 otherwise
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

    seq_t val = (seq_t)n * 5;
    idx_t r = n % 10;

    if (r == 1 || r == 4)
        val -= (seq_t)1;
    else if (r == 6 || r == 9)
        val += (seq_t)1;

    return val;
}
