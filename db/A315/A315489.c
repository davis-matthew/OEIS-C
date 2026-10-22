/*
 * A315489  Coordination sequence Gal.5.292.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Coordination sequence for Gal.5.292.5.
   a(0) = 1; for 1 ≤ n ≤ 10: a(n) = 5*n + (n+2)/3.
   For n ≥ 11 the differences repeat with period 10:
   [6, 5, 5, 6, 5, 5, 6, 5, 5, 6].
   Equivalently,
     a(n) = 54 + 54*floor((n-10)/10) + Σ_{k=0}^{(n-10)%10-1} p_k
   where p_k are the terms of the period above.
*/

seq_t a(idx_t n)
{
    /* handle n == 0 */
    if (n == 0) return (seq_t)1;

    /* 1 <= n <= 10 */
    if (n <= 10) {
        seq_t v = (seq_t)n;
        return v * (seq_t)5 + (v + (seq_t)2) / (seq_t)3;
    }

    /* n >= 11 : use periodic pattern */
    const seq_t a10 = (seq_t)10 * (seq_t)5 + ((seq_t)10 + (seq_t)2) / (seq_t)3; /* 54 */
    const seq_t period_sum = (seq_t)54;                                   /* sum of one period */
    static const seq_t P[10] = {6, 5, 5, 6, 5, 5, 6, 5, 5, 6};

    idx_t m = n - (idx_t)10;
    idx_t block = m / (idx_t)10;
    idx_t rem   = m % (idx_t)10;

    seq_t result = a10;
    result += period_sum * (seq_t)block;

    for (idx_t i = 0; i < rem; ++i)
        result += P[i];

    return result;
}
