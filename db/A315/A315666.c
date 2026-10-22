/*
 * A315666  Coordination sequence Gal.6.134.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(1) = 6; for n ≥ 2: a(n) = a(n‑1) + diff[(n‑2) mod
 *            12], where diff = {6,5,4,4,4,4,4,4,5,6,6,6}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A315666: Coordination sequence Gal.6.134.6.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* Differences for the ring sizes, period 12. */
    static const seq_t diff[12] = {6,5,4,4,4,4,4,4,5,6,6,6};

    /* Number of difference terms to apply after a(1).  */
    idx_t steps = n - 1;          /* n‑1 terms beyond the first.  */
    idx_t cycles = steps / 12;    /* Full 12‑step cycles.           */
    idx_t rest   = steps % 12;    /* Remaining terms.               */

    /* Sum of one full cycle.  */
    const seq_t cycle_sum = 58;   /* 6+5+4+4+4+4+4+4+5+6+6+6 */

    seq_t result = (seq_t)6;      /* a(1) */
    result += (seq_t)cycles * cycle_sum;

    for (idx_t i = 0; i < rest; ++i)
        result += diff[i];

    return result;
}
