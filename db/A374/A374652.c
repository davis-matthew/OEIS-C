/*
 * A374652  a(n) = Sum_{k=1..n+1} (-1)^(k - 1)*(k + 1)*(n - k + 1)!.
 * https://oeis.org/A374652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n+1} (-1)^(k-1)*(k+1)*(n-k+1)!
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum  = 0;
    seq_t fact = 1;               /* j! for j = 0 initially */
    for (idx_t j = 0; j <= n; ++j) {
        seq_t factor = (seq_t)(n - j + 2);   /* (n-j+2) */
        seq_t term   = fact * factor;
        if ((n - j) & 1) {                  /* (-1)^(n-j) factor */
            term = -term;
        }
        sum += term;
        fact *= (seq_t)(j + 1);             /* update j! to (j+1)! */
    }
    return sum;
}
