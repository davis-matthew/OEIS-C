/*
 * A382446  a(n) = Sum_{k=0..n} binomial(n,k) * ( binomial(n,k) -
 *          binomial(n,k-1) )^6.
 * https://oeis.org/A382446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * ( binomial(n,k) -
 *            binomial(n,k-1) )^6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t binom_prev = 1;           /* binomial(n,0) */
    /* k = 0 term: binom = 1, difference = 1, 1^6 = 1 */
    sum += 1;
    for (idx_t k = 1; k <= n; ++k) {
        /* binom(n,k) = binom(n,k-1) * (n-k+1) / k */
        seq_t numerator   = binom_prev * (seq_t)(n - k + 1);
        seq_t binom_curr  = numerator / (seq_t)k;

        /* difference = |binom_curr - binom_prev| */
        seq_t diff = (binom_curr > binom_prev)
                     ? (binom_curr - binom_prev)
                     : (binom_prev - binom_curr);

        /* diff^6 = diff^4 * diff^2  */
        seq_t diff2 = diff * diff;          /* diff^2 */
        seq_t diff4 = diff2 * diff2;        /* diff^4 */
        seq_t diff6 = diff4 * diff * diff;  /* diff^6 */

        /* term = binom_curr * diff^6 */
        seq_t term = binom_curr * diff6;
        sum += term;

        binom_prev = binom_curr;
    }
    return sum;
}
