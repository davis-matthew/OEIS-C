/*
 * A311554  Coordination sequence Gal.6.134.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0)=1, a(1)=4, a(2)=8, a(3)=12, for n>=4: a(n) = a(n-1) + d(n),
 *            where d(n) repeats every 12 with pattern
 *            {6,6,6,6,5,4,4,4,4,4,4,5}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values for n < 4 */
    static const seq_t small[4] = {1, 4, 8, 12};

    if (n < 4)
        return small[n];

    /* increments d(5) .. d(16) (12 elements, period 12) */
    static const seq_t inc[12] = {
        6, 6, 6, 6, /* d(5) .. d(8) */
        5,          /* d(9)   */
        4, 4, 4, 4, 4, 4, /* d(10) .. d(15) */
        5           /* d(16)   */
    };

    const seq_t a4 = 17;      /* a(4) */
    const seq_t period_sum = 58; /* sum of inc[] */

    /* number of steps beyond a(4) */
    idx_t m = n - 4;
    idx_t full = m / 12;
    idx_t rem = m % 12;

    seq_t result = a4;
    result += (seq_t)full * period_sum;

    /* add remaining increments */
    for (idx_t i = 0; i < rem; ++i)
        result += inc[i];

    return result;
}
