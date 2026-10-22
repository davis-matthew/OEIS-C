/*
 * A315258  Coordination sequence Gal.6.260.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 58 + 58*⌊(n-12)/12⌋ + p[(n-12) mod 12] (n ≥ 12), where
 *            p[0..12] = {0,6,10,15,20,25,29,33,38,43,48,52,58}. Initial
 *            values: a(0)=1, a(1)=6, a(2)=10, a(3)=15, a(4)=20, a(5)=25,
 *            a(6)=29, a(7)=33, a(8)=38, a(9)=43, a(10)=48, a(11)=52, a(12)=58
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first 13 terms (0 … 12) */
    static const seq_t pre[13] = {
        1ULL, 6ULL, 10ULL, 15ULL, 20ULL, 25ULL, 29ULL, 33ULL,
        38ULL, 43ULL, 48ULL, 52ULL, 58ULL
    };

    /* prefix sums of the 12‑term difference pattern {6,4,5,5,5,4,4,5,5,5,4,6} */
    static const seq_t pref[13] = {
        0ULL, 6ULL, 10ULL, 15ULL, 20ULL, 25ULL, 29ULL, 33ULL,
        38ULL, 43ULL, 48ULL, 52ULL, 58ULL
    };

    if (n < 13)
        return pre[n];

    idx_t count = n - 12;          /* number of steps beyond a(12) */
    idx_t whole = count / 12;       /* whole 12‑step blocks */
    idx_t rem   = count % 12;       /* remainder steps within a block */

    seq_t result = 58;                      /* a(12) */
    result += (seq_t)whole * 58;            /* add complete 12‑step blocks */
    result += pref[rem];                    /* add the remaining partial block */
    return result;
}
