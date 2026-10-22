/*
 * A315185  Coordination sequence Gal.5.115.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS formula: 
   a(0) = 1
   a(n) = 1 + 5 + 42*floor((n-1)/9) + Σ_{k=0}^{((n-1)%9)-1} p_k,   n ≥ 1
         where  p = [4,4,4,6,4,4,4,6,6]  (period 9 means p_k is taken modulo 9)
*/

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* Periodic difference pattern after the first step */
    static const seq_t pattern[9] = {4,4,4,6,4,4,4,6,6};
    const seq_t pattern_sum = 42;          /* sum of one full period */

    idx_t m = n - 1;                       /* number of diffs after the first one */
    idx_t cycles = m / 9;
    idx_t rem = m % 9;

    /* Compute a(n) by summing the constant contributions */
    seq_t sum = 1;                         /* start with a(0) */
    sum += 5;                              /* diff[1] */
    sum += (seq_t)cycles * pattern_sum;    /* complete periods */

    for (idx_t i = 0; i < rem; ++i)
        sum += pattern[i];                 /* remaining partial period */

    return sum;
}
