/*
 * A386898  a(n) = Sum_{k=0..n} binomial(5*n+1,k) * binomial(5*n-k,n-k).
 * https://oeis.org/A386898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(5*n+1,k) * binomial(5*n-k,n-k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom = 1;   /* binomial(4*n + 0, 0) */
    seq_t pow2 = 1;    /* 2^0 */

    for (idx_t k = 0; k <= n; ++k) {
        /* accumulate current term */
        sum += pow2 * binom;

        if (k == n) break;  /* finished loop */

        /* Update binom to binomial(4*n + k + 1, k + 1) */
        seq_t numerator = (seq_t)(4 * n + k + 1);
        binom = binom * numerator / (seq_t)(k + 1);

        /* Update pow2 to 2^(k+1) */
        pow2 <<= 1;  /* multiply by 2 */
    }

    return sum;
}
