/*
 * A311964  Coordination sequence Gal.6.131.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n)=4 + Σ_{k=0}^{n-2} pattern[k mod 12] where
 *            pattern = {4,5,4,5,5,5,5,4,5,4,4,4}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined by aperiodic initial difference a(1)-a(0)=3 and
   thereafter by the period 12 pattern. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* period-12 difference pattern after the first step (3) */
    static const idx_t pattern[12] = {4,5,4,5,5,5,5,4,5,4,4,4};
    const seq_t period_sum = (seq_t)54;   /* 4+5+4+5+5+5+5+4+5+4+4+4 */

    seq_t result = (seq_t)4;              /* 1 (a(0)) + 3 (first diff) */
    idx_t terms   = n - 1;                /* number of pattern terms to add */
    idx_t full    = terms / 12;           /* complete period repetitions */
    idx_t rem     = terms % 12;           /* remaining terms of the period */

    result += (seq_t)full * period_sum;   /* add full periods */
    for (idx_t i = 0; i < rem; ++i)       /* add remaining terms */
        result += (seq_t)pattern[i];

    return result;
}
