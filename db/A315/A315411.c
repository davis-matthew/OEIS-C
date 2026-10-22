/*
 * A315411  Coordination sequence Gal.4.78.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6. For n ≥ 2: if n is even: if n==2: a(n)=a(n-2)+10
 *            else: a(n)=a(n-2)+9 if ((n/2-2)%4<2) else a(n)=a(n-2)+11 if n is
 *            odd: let m=(n-1)/2; a(n)=a(n-2)+9 if (m-1)%4==0 or 2,
 *            a(n)=a(n-2)+10 if (m-1)%4==1, a(n)=a(n-2)+12 if (m-1)%4==3 This
 *            recurrence reproduces the OEIS sequence A315411
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    seq_t a_prev2 = (seq_t)1;  /* a(n-2)  */
    seq_t a_prev1 = (seq_t)6;  /* a(n-1)  */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t a_i;
        if ((i & 1) == 0)               /* even i */
        {
            if (i == 2)
            {
                a_i = a_prev2 + (seq_t)10;   /* a(2) = a(0)+10 */
            }
            else
            {
                idx_t m = i >> 1;                   /* m = i/2  */
                idx_t mod = (m >= 2) ? ((m - 2) & 3) : 0;
                seq_t shift = (mod < 2) ? (seq_t)9 : (seq_t)11;
                a_i = a_prev2 + shift;
            }
        }
        else                            /* odd i */
        {
            idx_t m = (i - 1) >> 1;           /* m = (i-1)/2 */
            idx_t mod = (m > 0) ? ((m - 1) & 3) : 0;
            seq_t shift;
            if (mod == 0 || mod == 2)   shift = (seq_t)9;
            else if (mod == 1)          shift = (seq_t)10;
            else                        shift = (seq_t)12;  /* mod == 3 */
            a_i = a_prev2 + shift;
        }

        /* shift window for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_i;
    }

    return a_prev1;
}
