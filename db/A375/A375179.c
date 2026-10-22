/*
 * A375179  a(n) = Sum_{k = 0..n-1} (-1)^(n+k+1)*binomial(2*n, k)^3.
 * https://oeis.org/A375179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n-1} (-1)^(n+k+1) * binomial(2*n, k)^3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(0) = 0. */
    if (n == 0) {
        return 0;
    }

    idx_t N2 = 2 * n;                 /* 2*n as an idx_t for later use */
    seq_t binom = 1;                  /* binomial(2n, 0) */
    seq_t pos_sum = 0;                /* Sum of positive terms   */
    seq_t neg_sum = 0;                /* Sum of negative terms   */

    /* Loop over k = 0 .. n-1 */
    for (idx_t k = 0; k < n; ++k) {
        /* Compute binomial(2n, k)^3 */
        seq_t term = binom;
        term = term * binom;          /* binom^2   */
        term = term * binom;          /* binom^3   */

        /* Determine the sign: (-1)^(n + k + 1) */
        if (((n + k + 1) & 1) == 0) {
            pos_sum += term;
        } else {
            neg_sum += term;
        }

        /* Prepare binomial(2n, k+1) for next iteration, if needed */
        if (k + 1 < n) {
            seq_t prod = binom * (seq_t)(N2 - k);
            binom = prod / (seq_t)(k + 1);
        }
    }

    /* Final result: positive contributions minus negative ones */
    return pos_sum - neg_sum;
}
