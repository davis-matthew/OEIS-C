/*
 * A354333  a(n) is the denominator of Sum_{k=0..n} (-1)^k / (2*k+1)!.
 * https://oeis.org/A354333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) is the denominator of Sum_{k=0..n} (-1)^k / (2*k+1)! (in
 *            lowest terms)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* fac   = (2n+1)!   */
    /* T     = (2n+1)! * S_n = Sum_{k=0}^n (-1)^k * (2n+1)!/(2k+1)! */
    seq_t fac = 1;
    seq_t T   = 1;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* multiply fac and T by (2i)(2i+1) */
        seq_t two_i = (seq_t)2 * i;           /* 2*i  (fits in seq_t) */
        seq_t mul   = two_i * (two_i + 1);    /* (2i)(2i+1) -- also fits in seq_t */

        fac *= mul;
        T   *= mul;

        /* add or subtract 1 according to the sign (-1)^i */
        if ((i & 1) == 0)          /* i even -> +1 */
            T += (seq_t)1;
        else                       /* i odd  -> -1 */
            T -= (seq_t)1;
    }

    /* gcd of T and fac gives G_n; the denominator is fac/G_n */
    seq_t g = T;
    seq_t h = fac;
    while (h != 0)
    {
        seq_t r = g % h;
        g = h;
        h = r;
    }

    return fac / g;
}
