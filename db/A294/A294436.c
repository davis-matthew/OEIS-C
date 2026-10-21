/*
 * A294436  a(n) = Sum_{m=0..n} (Sum_{k=0..m} binomial(n,k))^5.
 * https://oeis.org/A294436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=0..n} (Sum_{k=0..m} binomial(n,k))^5
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t sum   = 0;
    seq_t c     = 1;  /* C(n,0) */

    /* m = 0 */
    sum += c;
    seq_t tmp  = sum * sum;          /* sum^2   */
    seq_t pow5 = tmp * tmp * sum;     /* sum^5   */
    total += pow5;

    for (idx_t k = 1; k <= n; ++k) {
        /* C(n,k) = C(n,k-1) * (n-k+1) / k  */
        c = (c * (seq_t)(n - k + 1)) / (seq_t)k;
        sum += c;

        tmp  = sum * sum;            /* sum^2   */
        seq_t pow4 = tmp * tmp;      /* sum^4   */
        pow5 = pow4 * sum;           /* sum^5   */
        total += pow5;
    }
    return total;
}
