/*
 * A314252  Coordination sequence Gal.5.295.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 6*n - 1 for 1 ≤ n ≤ 9 a(n) = 53 +
 *            floor((n-9)/10)*58 + {0,5,10,16,22,28,34,40,46,52}[ (n-9) mod 10
 *            ] for n ≥ 10 (The second part is derived from a periodic
 *            difference sequence: 5,5,6,6,6,6,6,6,6,6 repeating every 10
 *            terms.)
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n <= 9) return (seq_t)6 * n - (seq_t)1;

    /* n >= 10 */
    idx_t m = n - 9;                         /* number of diff steps after a(9) */
    idx_t cycles = m / 10;
    idx_t rem = m % 10;

    static const seq_t prefix_sum[11] = {
        0ULL, /* 0 terms */
        5ULL, /* 5     */
        10ULL,/* 5+5   */
        16ULL,/* 5+5+6 */
        22ULL,
        28ULL,
        34ULL,
        40ULL,
        46ULL,
        52ULL,
        58ULL  /* full period sum */
    };

    const seq_t sum_period = 58;            /* sum of the 10-period diff sequence */
    seq_t result = (seq_t)53;               /* a(9) value */

    result += (seq_t)cycles * sum_period;
    result += prefix_sum[rem];

    return result;
}
