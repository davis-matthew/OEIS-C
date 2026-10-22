/*
 * A394488  Expansion of e.g.f. (1/x) * Series_Reversion( x/(1 + 5*x)^(1/5) ).
 * https://oeis.org/A394488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Π_{i=0}^{n-1} (n+1-5*i) / (n+1) Simplifies to a(n) =
 *            Π_{i=1}^{n-1} (n+1-5*i) (for n ≥ 1) and a(0) = 1
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = n+1  (stored as seq_t to allow arbitrary‑precision arithmetic) */
    seq_t m = (seq_t)n + 1;

    /* a(0) = 1 explicitly, because the product over an empty index set
       is the multiplicative identity. The loop below would not be executed
       for n=0 anyway, but the explicit check avoids converting 0 to a
       seq_t for efficiency on narrow integer builds. */
    if (n == 0)
        return (seq_t)1;

    /* Compute the product of the terms (m - 5*i) for i = 1 .. n-1.
       The factor for i = 0 is exactly m and cancels the division by (n+1),
       so it is omitted from the loop. */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
    {
        seq_t term = m - (seq_t)(5 * i);
        result *= term;
    }
    return result;
}
