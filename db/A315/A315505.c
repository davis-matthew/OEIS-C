/*
 * A315505  Coordination sequence Gal.3.59.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + 1 + f(n) where f(0) = 0 f(n) for n%4==0 and n>0 =
 *            4*(n/4) - 1 f(n) for n%4!=0 = 2*floor(n/4) This reproduces the
 *            OEIS sequence A315505 (Coordination sequence Gal.3.59.2)
 * Range:     u8 n <= 43, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                     /* a(0) = 1 */
        return 1;

    idx_t k = n / 4;                 /* floor(n/4) */
    int   rem = n % 4;               /* n mod 4, small enough for int */

    /* base value 5*n + 1 */
    seq_t base = (seq_t)5 * (seq_t)n + 1;

    seq_t add;
    if (rem == 0)                    /* n = 4k, k > 0 */
    {
        if (k == 0)                  /* happens only for n=0, handled above */
            add = 0;
        else
            add = (seq_t)4 * (seq_t)k - 1;   /* 4k - 1 */
    }
    else                             /* n = 4k + 1, 2, or 3 */
    {
        add = (seq_t)2 * (seq_t)k;              /* 2k */
    }

    return base + add;
}
