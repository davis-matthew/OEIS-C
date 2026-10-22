/*
 * A314149  Coordination sequence Gal.6.620.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 5.
   For n ≥ 2, a(n) = a(n‑1) + 5 + ε(n),
   where ε(n) = 1 iff ((n-2)/11) and n have the same parity
   and n % 11 ≠ 1; otherwise ε(n) = 0. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) */

    seq_t res = (seq_t)5;         /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t inc = 5;            /* default increment */

        if ((i % 11) != 1ULL)    /* special case  i % 11 == 1  -> keep 5 */
        {
            idx_t group = (i - 2) / 11;   /* floor((i-2)/11) */
            if ((group & 1ULL) == (i & 1ULL))
                inc = 6;                 /* add the extra 1 */
        }

        res += inc;
    }
    return res;
}
