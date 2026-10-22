/*
 * A335310  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+k,k) * (-n)^(n-k).
 * https://oeis.org/A335310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^2 * (1-n)^k
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 16, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;                           /* running total                           */
    seq_t bin = 1;                           /* current C(n,k)                          */
    seq_t pow = 1;                           /* current (1-n)^k                         */
    seq_t base = (seq_t)1 - (seq_t)n;        /* 1-n                                     */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = bin * bin;              /* C(n,k)^2                                 */
        term *= pow;                         /* * (1-n)^k                                */
        sum += term;                         /* accumulate                                */

        if (k == n) break;                   /* last iteration, nothing more to update   */

        /* update C(n,k+1) = C(n,k) * (n-k)/(k+1)  */
        bin = bin * ((seq_t)n - (seq_t)k);
        bin = bin / ((seq_t)k + 1);

        /* update pow = pow * (1-n)  */
        pow = pow * base;
    }

    return sum;
}
