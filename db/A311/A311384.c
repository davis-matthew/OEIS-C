/*
 * A311384  Coordination sequence Gal.6.220.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1: a(n) = 4·n + 2·(⌊(n+5)/10⌋ + ⌊(n+4)/10⌋)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t N = (seq_t)n;
    /* Count of indices i in [2..n] with i≡5 or 6 (mod 10) */
    seq_t f = ((seq_t)(n + 5)) / 10 + ((seq_t)(n + 4)) / 10;

    return 4 * N + 2 * f;
}
