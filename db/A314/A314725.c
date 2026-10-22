/*
 * A314725  Coordination sequence Gal.5.114.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1)=5, a(2)=9, a(3)=13; for n ≥ 4, a(n) = a(n-1) + 5
 *            if n % 10 ∈ {0,1,4,5,6,7}, otherwise a(n) = a(n-1) + 4
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return 1;
    if (n == 1u) return 5;
    if (n == 2u) return 9;
    if (n == 3u) return 13;

    seq_t val = 13;
    for (idx_t k = 4; k <= n; ++k) {
        idx_t r = k % 10ull;
        seq_t d = (r == 0ull || r == 1ull || r == 4ull || r == 5ull || r == 6ull || r == 7ull) ? 5 : 4;
        val += d;
    }
    return val;
}
