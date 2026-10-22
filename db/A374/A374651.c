/*
 * A374651  a(n) = Sum_{k=1..n+1} (-1)^(k-1) k (n - k + 1)!.
 * https://oeis.org/A374651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n+1} (-1)^(k-1) * k * (n - k + 1)!
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum  = 0;       /* accumulated result                                  */
    seq_t fact = 1;       /* current factorial (t! with t from 0 to n)           */
    seq_t sign = 1;       /* (-1)^(n - t); alternates each iteration            */
    if (n & 1ULL) {
        sign = -1;        /* if n is odd the initial sign is -1                */
    }

    for (idx_t t = 0; t <= n; ++t) {
        seq_t k = (seq_t)(n - t + 1);      /* k = n - t + 1                           */
        sum += fact * (sign * k);          /* add current term to the sum             */
        fact *= (seq_t)(t + 1);            /* prepare (t+1)! for next iteration      */
        sign = -sign;                      /* flip sign for next term                 */
    }

    return sum;
}
