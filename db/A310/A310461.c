/*
 * A310461  Coordination sequence Gal.6.350.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, for n≥2 * a(n) = 4 + Σ_{k=2}^{n} d(k), where the
 *            6‑periodic increments d(k) are * {4,4,6,5,5,6} corresponding to
 *            k mod 6 = {0,1,2,3,4,5}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    /* Periodic increment pattern for k mod 6. */
    static const seq_t inc[6] = {4, 4, 6, 5, 5, 6};

    /* Number of terms to sum: k = 2 … n inclusive. */
    idx_t cnt = n - 1;           /* cnt ≥ 1 for n ≥ 2 */
    idx_t full_blocks = cnt / 6;
    idx_t rem = (idx_t)(cnt % 6);

    /* Sum of one full period: 4+4+6+5+5+6 = 30. */
    const seq_t period_sum = (seq_t)30;

    /* Start the remainder from k = 2 => residue 2. */
    idx_t start = 2 % 6;           /* = 2 */
    seq_t sum = (seq_t)full_blocks * period_sum;

    for (idx_t i = 0; i < rem; ++i)
        sum += inc[(start + i) % 6];

    return (seq_t)4 + sum;
}
