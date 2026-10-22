/*
 * A382443  a(n) = Sum_{k=0..n} binomial(n,k) * ( binomial(n,k) -
 *          binomial(n,k-1) )^4.
 * https://oeis.org/A382443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * ( binomial(n,k) -
 *            binomial(n,k-1) )^4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom = 1;           /* C(n,0) */
    seq_t prev_binom = 0;      /* C(n,-1) = 0 */
    /* k = 0 */
    seq_t diff = binom;               /* diff = C(n,0) - 0 */
    seq_t diff2 = diff * diff;
    seq_t diff4 = diff2 * diff2;
    sum += binom * diff4;
    prev_binom = binom;

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binom to C(n,k) = C(n,k-1) * (n-k+1) / k */
        binom = binom * (seq_t)(n - k + 1) / (seq_t)k;
        diff = binom - prev_binom;    /* diff = C(n,k) - C(n,k-1) */
        diff2 = diff * diff;
        diff4 = diff2 * diff2;
        sum += binom * diff4;
        prev_binom = binom;
    }
    return sum;
}
