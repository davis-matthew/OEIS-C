/*
 * A385444  Expansion of e.g.f. (1/x) * Series_Reversion( x/(4*x +
 *          sqrt(16*x^2+1))^(1/4) ).
 * https://oeis.org/A385444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∏_{j=1}^{n-1} (5*n + 1 - 8*j) , a(0)=1
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 9, i64 n <= 15, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Zero occurs when (5*n+1) is a multiple of 8, i.e. n ≡ 3 (mod 8). */

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0) return (seq_t)1;

    /* d = 5*n + 1, computed in seq_t to avoid overflow */
    seq_t d = (seq_t)n * (seq_t)5 + (seq_t)1;

    /* If (5*n+1) is divisible by 8, one of the factors is zero */
    if ((d % (seq_t)8) == (seq_t)0)
        return (seq_t)0;

    /* Compute the product ∏_{j=1}^{n-1} (d - 8*j) */
    seq_t prod = (seq_t)1;
    for (idx_t j = 1; j < n; ++j)
    {
        seq_t term = d - (seq_t)8 * (seq_t)j;
        prod *= term;
    }
    return prod;
}
