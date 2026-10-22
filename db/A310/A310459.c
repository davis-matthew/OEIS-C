/*
 * A310459  Coordination sequence Gal.6.341.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = 1 + 3 + Σ_{k=2}^{n} L(k), where the
 *            increment sequence L(k) is periodic with period 12: L(k) =
 *            [6,5,5,5,6,6,5,5,5,6,4,4] for k ≥ 2. The sum of one full period
 *            is 62
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

    /* Periodic increments for k ≥ 2 */
    static const seq_t period[12] = {6,5,5,5,6,6,5,5,5,6,4,4};
    const seq_t period_sum = 62;  /* 6+5+5+5+6+6+5+5+5+6+4+4 */

    seq_t inc_sum = 3;                      /* L(1) = 3 */
    idx_t t = n - 1;                        /* number of terms from k = 2 to n inclusive */
    idx_t full = t / 12;                    /* full periods */
    idx_t rem  = t % 12;                    /* remaining terms */

    inc_sum += (seq_t)full * period_sum;    /* add full periods */

    /* add the remaining part of the period */
    for (idx_t i = 0; i < rem; ++i)
        inc_sum += period[i];

    return 1 + inc_sum;                     /* a(0) + Σ L(k) for k = 1..n */
}
