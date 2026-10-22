/*
 * A376459  a(n) = Sum_{k = 0..n} (-1)^(n+k)*binomial(n, k)*binomial(n+k,
 *          k)*A108625(n, n-k).
 * https://oeis.org/A376459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k = 0..n} (-1)^k * binomial(n,k)^3 * binomial(n+k,k)
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 9, i64 n <= 18, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Uses multiplicative updates for the binomial coefficients            */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t Cnk = 1;   /* binomial(n,0)   */
    seq_t Bk  = 1;   /* binomial(n+0,0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = (-1)^k * C(n,k)^3 * C(n+k,k) */
        seq_t term = Cnk * Cnk;   /* Cnk^2 */
        term = term * Cnk;        /* Cnk^3 */
        term = term * Bk;         /* times C(n+k,k) */
        if (k & 1)
            sum -= term;
        else
            sum += term;

        /* update for next k, if any */
        if (k < n) {
            seq_t num1 = (seq_t)(n - k);
            seq_t den1 = (seq_t)(k + 1);
            Cnk = (Cnk * num1) / den1;

            seq_t num2 = (seq_t)(n + k + 1);
            seq_t den2 = (seq_t)(k + 1);
            Bk = (Bk * num2) / den2;
        }
    }
    return sum;
}
