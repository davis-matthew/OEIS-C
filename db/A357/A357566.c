/*
 * A357566  a(n) = ( Sum_{k = 0..n} binomial(n+k-1,k)^2 )^3 * ( Sum_{k = 0..n}
 *          binomial(n+k-1,k)^3 )^2.
 * https://oeis.org/A357566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( Sum_{k = 0..n} binomial(n+k-1,k)^2 )^3 * ( Sum_{k =
 *            0..n} binomial(n+k-1,k)^3 )^2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th element of OEIS A357566. */
seq_t a(idx_t n)
{
    seq_t term = 1;       /* term = binomial(n+k-1, k), starting k = 0 */
    seq_t sum2  = 0;      /* Σ term^2   for k = 0..n */
    seq_t sum3  = 0;      /* Σ term^3   for k = 0..n */

    idx_t k;
    for (k = 0; k <= n; ++k)
    {
        seq_t t2 = term * term;      /* term^2   */
        sum2 += t2;                  /* add to Σ term^2   */
        sum3 += t2 * term;           /* add term^3        */

        /* update term for next k, except after the last iteration */
        if (k != n)
        {
            term = term * ((seq_t)(n + k)) / ((seq_t)(k + 1));
        }
    }

    /* a(n) = sum2^3 * sum3^2  */
    seq_t cube  = sum2 * sum2 * sum2;   /* (Σ term^2)^3  */
    seq_t square = sum3 * sum3;         /* (Σ term^3)^2  */
    return cube * square;              /* product  */
}
