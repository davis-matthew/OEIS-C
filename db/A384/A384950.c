/*
 * A384950  a(n) = Sum_{k=0..n} 3^k * binomial(2*n+k-1,k).
 * https://oeis.org/A384950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binomial(2*n + k - 1, k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum   = 0;          /* accumulator for the sum */
    seq_t binom = 1;          /* binomial(2*n - 1, 0) = 1 */
    seq_t pow3  = 1;          /* 3^0 */
    for (idx_t k = 0; k <= n; ++k) {
        sum += pow3 * binom;  /* add the current term */
        if (k == n) break;    /* finished all terms */
        /* Update binomial to C(2*n + k, k+1) */
        seq_t mult = (seq_t)(2 * n + k); /* 2*n + k  (fits in idx_t) */
        binom = binom * mult / (k + 1);
        pow3 *= 3;            /* advance 3^k to 3^{k+1} */
    }
    return sum;
}
