/*
 * A315523  Coordination sequence Gal.5.301.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6, a(2)=11; For n≥3 a(n) = a(n-2) + 11 + (n % 9 ==
 *            1 ? 1 : 0)
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;
    if (n == 2)
        return (seq_t)11;

    /* a_prev2 holds a(n-2), a_prev1 holds a(n-1) during the loop */
    seq_t a_prev2 = (seq_t)6;  /* a(1) */
    seq_t a_prev1 = (seq_t)11; /* a(2) */
    seq_t a_new   = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        a_new = a_prev2 + (seq_t)11 + ((i % 9 == 1) ? (seq_t)1 : (seq_t)0);
        a_prev2 = a_prev1;
        a_prev1 = a_new;
    }

    return a_new;
}
