/*
 * A310533  Coordination sequence Gal.6.334.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4; for n>=2: a(n) = 4 + 62*floor((n-1)/11) +
 *            partial[(n-1)%11] where partial[0..10] =
 *            {0,6,12,18,24,30,36,42,48,54,58}
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes oeis A310533: Coordination sequence Gal.6.334.1 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* number of steps from k=2 to k=n (inclusive) */
    idx_t steps = n - 1;
    idx_t cycles = steps / 11;        /* complete periods of length 11 */
    idx_t rem    = steps % 11;        /* remaining steps in the last period */

    /* partial sums of the differences for the first r steps of a period:
       partial[r] = Σ_{k=2}^{2+r-1} d(k),  d(k)=4 if k%11==0 or 1, else 6 */
    static const seq_t partial[11] = {
        (seq_t)0,   /* r=0   */
        (seq_t)6,   /* r=1   */
        (seq_t)12,  /* r=2   */
        (seq_t)18,  /* r=3   */
        (seq_t)24,  /* r=4   */
        (seq_t)30,  /* r=5   */
        (seq_t)36,  /* r=6   */
        (seq_t)42,  /* r=7   */
        (seq_t)48,  /* r=8   */
        (seq_t)54,  /* r=9   */
        (seq_t)58   /* r=10  */
    };

    /* compute the accumulated sum of differences */
    seq_t sum = (seq_t)cycles * (seq_t)62;
    sum += partial[rem];

    return (seq_t)4 + sum;
}
