/*
 * A383996  a(n) = Product_{k=0..n-1} (n-4*k).
 * https://oeis.org/A383996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Product_{k=0..n-1} (n-4*k) = ∏_{k=0}^{n-1} (n-4k)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t k = 0; k < n; ++k)
    {
        seq_t term = (seq_t)n - (seq_t)4 * (seq_t)k;
        prod *= term;
    }
    return prod;
}
