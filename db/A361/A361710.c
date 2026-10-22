/*
 * A361710  a(n) = Sum_{k = 0..n-1} (-1)^k*binomial(n,k)*binomial(n-1,k)^2.
 * https://oeis.org/A361710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..n-1} (-1)^k*binomial(n,k)*binomial(n-1,k)^2
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 16, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) return 0;

    /* iterative computation of binomials to avoid repeated factorials */
    seq_t bn  = 1;  /* binomial(n,0)      */
    seq_t bn1 = 1;  /* binomial(n-1,0)    */

    for (idx_t k = 0; k < n; ++k)
    {
        seq_t term = bn * bn1 * bn1;  /* C(n,k) * C(n-1,k)^2   */
        if (k & 1) term = -term;     /* apply (-1)^k         */
        sum += term;

        /* prepare next binomial values if not last iteration */
        if (k + 1 < n)
        {
            seq_t num  = (seq_t)(n - k);          /* n - k          */
            seq_t den  = (seq_t)(k + 1);          /* k + 1          */
            bn  = bn  * num / den;                /* C(n,k+1)       */
            seq_t num1 = (seq_t)(n - 1 - k);       /* n-1 - k        */
            bn1 = bn1 * num1 / den;                /* C(n-1,k+1)     */
        }
    }
    return sum;
}
