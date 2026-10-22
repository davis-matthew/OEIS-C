/*
 * A312983  Coordination sequence Gal.3.25.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=9, a(3)=13, a(4)=20, a(5)=24, a(6)=28. For
 *            n ≥ 7 the sequence obeys a difference cycle of length 5: 6, 4,
 *            6, 4, 4. Thus a(n) = 28 + 24·⌊(n‑6)/5⌋ + (sum of the first (n‑6)
 *            mod 5 terms of {6,4,6,4,4})
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed values for n = 0 … 6 */
    static const seq_t pre[] = {1, 4, 9, 13, 20, 24, 28};

    /* Return a pre‑computed value if the index is small */
    if (n < (idx_t)(sizeof(pre) / sizeof(pre[0])))
        return pre[n];

    /* Cycle of differences for n ≥ 7 */
    static const seq_t cycle[5] = {6, 4, 6, 4, 4};
    const seq_t base = 28;          /* a(6) */
    const seq_t cycle_sum = 24;     /* 6+4+6+4+4 */

    idx_t m = n - 6;                /* number of differences from a(6) to a(n) */
    idx_t cycles = m / 5;           /* complete 5‑term cycles */
    idx_t rem = m % 5;              /* remaining (partial) cycle terms */

    seq_t result = base;
    result += (seq_t)cycles * cycle_sum; /* add complete cycle sums */

    /* Add the remaining partial cycle terms */
    for (idx_t i = 0; i < rem; ++i)
        result += cycle[i];

    return result;
}
