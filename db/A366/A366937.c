/*
 * A366937  a(n) = Sum_{k=1..n} (-1)^(k-1) * binomial(k+1,2) * floor(n/k).
 * https://oeis.org/A366937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k-1) * binomial(k+1,2) * floor(n/k)
 * Range:     i8 n <= 19, i16 n <= 52, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* binomial(k+1, 2) = k*(k+1)/2 */
        seq_t binom = ((seq_t)k * ((seq_t)k + 1)) / 2;
        idx_t m = n / k;                     /* floor(n/k) */
        seq_t term = binom * (seq_t)m;        /* binomial * floor */
        if ((k & 1) == 0)                    /* k even: sign = -1 */
            sum -= term;
        else                                 /* k odd: sign = +1 */
            sum += term;
    }
    return sum;
}
