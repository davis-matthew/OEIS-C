/*
 * A377397  Expansion of e.g.f. (1 + log(1+x))^4.
 * https://oeis.org/A377397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..4} k! * binomial(4,k) * Stirling1(n,k)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 the sequence has value 1 */
    if (n == 0u) return (seq_t)1;

    /* Constants: factorials and binomial coefficients for k=0..4 */
    static const seq_t FACT[5] = { (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)6,
                                  (seq_t)24 };
    static const seq_t BIN[5]  = { (seq_t)1, (seq_t)4, (seq_t)6,
                                  (seq_t)4, (seq_t)1 };

    /* s_prev[k] holds Stirling1(n-1,k); s_curr[k] will hold Stirling1(n,k) */
    seq_t s_prev[5] = { (seq_t)1, 0, 0, 0, 0 };
    seq_t s_curr[5];

    /* Build up to n */
    for (idx_t m = 1; m <= n; ++m) {
        s_curr[0] = (seq_t)0;          /* Stirling1(m,0) = 0 for m>0 */
        for (idx_t k = 1; k <= 4; ++k) {
            seq_t term = s_prev[k - 1];
            term -= (seq_t)(m - 1) * s_prev[k];
            s_curr[k] = term;
        }
        /* copy current row to previous for next iteration */
        for (idx_t k = 0; k <= 4; ++k)
            s_prev[k] = s_curr[k];
    }

    /* Compute a(n) from the last Stirling numbers */
    seq_t result = (seq_t)0;
    for (idx_t k = 1; k <= 4; ++k)
        result += FACT[k] * BIN[k] * s_prev[k];

    return result;
}
