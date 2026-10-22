/*
 * A348515  a(n) = Sum_{d|n, d <= sqrt(n)} (-1)^(n/d + 1).
 * https://oeis.org/A348515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, d <= sqrt(n)} (-1)^(n/d + 1)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute floor(sqrt(n)) without overflow */
    idx_t sqrt_n = 0;
    while ((sqrt_n + 1) <= n / (sqrt_n + 1))
        ++sqrt_n;

    seq_t sum = 0;
    idx_t d;
    for (d = 1; d <= sqrt_n; ++d) {
        if (n % d == 0) {
            idx_t m = n / d;
            sum += ((m & 1) ? 1 : -1);  /* (-1)^(m+1) = 1 if m odd, else -1 */
        }
    }
    return sum;
}
