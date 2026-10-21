/*
 * A281430  Expansion of e.g.f. C(x)^4 where C(x) is described by A281428.
 * https://oeis.org/A281430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 4, for n ≥ 2: a(n) = ((36*n^2 - 45*n +
 *            16)*a(n-1) - 9*(n-1)*(2*n-3)*(3*n-2)*(3*n-4)*a(n-2)) / 2
 *            (rearranged to avoid overflow in 64‑bit builds)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)4;   /* a(1) */
    seq_t cur   = (seq_t)0;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t nn   = (seq_t)i;

        /* coefficients of the recurrence */
        seq_t t1 = (seq_t)36 * nn * nn
                   - (seq_t)45 * nn
                   + (seq_t)16;

        seq_t t2 = (seq_t)9 * (nn - (seq_t)1)
                   * (seq_t)(2 * nn - 3)
                   * (seq_t)(3 * nn - 2)
                   * (seq_t)(3 * nn - 4);

        /* split division by 2 to keep all intermediate values small
           for 64‑bit builds, while still being correct for all builds  */
        seq_t part1 = t1 * (prev1 / (seq_t)2);

        seq_t part2;
        if ((prev2 & (seq_t)1) == (seq_t)0) {
            /* a(n-2) is even – divide it first */
            part2 = t2 * (prev2 / (seq_t)2);
        } else {
            /* only possible for i = 2, here t2 is even */
            part2 = (t2 / (seq_t)2) * prev2;
        }

        cur   = part1 - part2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
