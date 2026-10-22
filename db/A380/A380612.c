/*
 * A380612  a(n) = (-1)^n*Product_{k=1..n} (2*k + 1)*(2*k - 3).
 * https://oeis.org/A380612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * Π_{k=1..n} ( (2*k+1)*(2*k-3) )
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t prod = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t term1 = (seq_t)(2 * (long long)k + 1);
        seq_t term2 = (seq_t)(2 * (long long)k - 3);
        prod *= term1;
        prod *= term2;
    }

    return (n & 1) ? -prod : prod;
}
