/*
 * A347994  a(n) = n! * Sum_{k=1..n-1} (-1)^(k+1) * n^(n-k-2) / (n-k-1)!.
 * https://oeis.org/A347994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=1..n-1} (-1)^(k+1) * n^(n-k-2) / (n-k-1)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent form used in the code:
   a(n) = (n-1)! * Σ_{j=0}^{n-2} (-1)^(n+j) * n^j / j!              */

seq_t a(idx_t n)
{
    if (n <= 1ULL)       /* a(1) = 0 by definition */
        return (seq_t)0;

    /* Compute (n-1)!  (loop works for any n ≥ 2) */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n - 1; ++i)
        fact *= (seq_t)i;

    /* term = (n-1)! / j! * n^j ; start with j = 0 */
    seq_t term = fact;
    int  sign = (n % 2 == 0) ? 1 : -1;   /* (-1)^(n+0) */
    seq_t pos = 0, neg = 0;

    if (sign > 0)   pos += term;
    else            neg += term;

    /* iterate j = 1 .. n-2  (term_{j+1} = term_j * n / (j+1)) */
    for (idx_t j = 1; j <= n - 2; ++j)
    {
        term = (term * (seq_t)n) / (seq_t)j;   /* exact division by j */
        sign = -sign;                           /* toggle sign */

        if (sign > 0) pos += term;
        else          neg += term;
    }

    return pos - neg;
}
