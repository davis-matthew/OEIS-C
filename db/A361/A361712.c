/*
 * A361712  a(n) = Sum_{k = 0..n-1}
 *          binomial(n,k)^2*binomial(n+k,k)*binomial(n+k-1,k).
 * https://oeis.org/A361712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n-1}
 *            binomial(n,k)^2*binomial(n+k,k)*binomial(n+k-1,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t sum = 0;
    seq_t bn = 1;    /* binomial(n,0)      */
    seq_t bk = 1;    /* binomial(n+k,k) at k=0 -> 1     */
    seq_t bm = 1;    /* binomial(n+k-1,k) at k=0 -> 1    */

    for (idx_t k = 0; k < n; ++k) {
        /* term = binomial(n,k)^2 * binomial(n+k,k) * binomial(n+k-1,k) */
        seq_t term = bn * bn;
        term = term * bk;
        term = term * bm;
        sum += term;

        if (k + 1 < n) {
            /* Update binomial(n,k+1)  = binomial(n,k) * (n-k)/(k+1) */
            bn = (bn * (seq_t)(n - k)) / (seq_t)(k + 1);

            /* Update binomial(n+k+1,k+1) = binomial(n+k,k+1) * (n+k+1)/(k+1) */
            bk = (bk * (seq_t)(n + k + 1)) / (seq_t)(k + 1);

            /* Update binomial(n+k,k+1)   = binomial(n+k-1,k+1) * (n+k)/(k+1) */
            bm = (bm * (seq_t)(n + k)) / (seq_t)(k + 1);
        }
    }

    return sum;
}
