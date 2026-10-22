/*
 * A310442  Coordination sequence Gal.6.327.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-11) + 54 for n ≥ 13. Initial values: a(0)=1, a(1)=4,
 *            a(2)=10, a(3)=15, a(4)=19, a(5)=24, a(6)=30, a(7)=35, a(8)=39,
 *            a(9)=44, a(10)=50, a(11)=54, a(12)=58
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[13] = {
        1ULL, 4ULL, 10ULL, 15ULL, 19ULL, 24ULL,
        30ULL, 35ULL, 39ULL, 44ULL, 50ULL, 54ULL, 58ULL
    };

    if (n <= 12u)
        return init[n];

    /* Reduce the index by multiples of 11 until it falls in [0,12]. */
    idx_t k = (n - 2u) / 11u;          /* number of 11‑step reductions */
    idx_t r = n - 11u * k;             /* remainder index (0 ≤ r ≤ 12) */

    return init[r] + (seq_t)54 * (seq_t)k;
}
