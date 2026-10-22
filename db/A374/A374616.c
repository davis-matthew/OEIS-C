/*
 * A374616  a(n) = Sum_{k=0..n} (k/n)^4 * binomial(n,k)^5.
 * https://oeis.org/A374616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n-1,k)^4 * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t result = 0;
    seq_t bin1 = 1; /* binomial(n-1,0) */
    seq_t bin2 = 1; /* binomial(n,0)   */

    for (idx_t k = 0; k < n; ++k) {
        /* term = bin1^4 * bin2 */
        seq_t sq   = bin1 * bin1;   /* bin1^2   */
        seq_t pow4 = sq * sq;       /* bin1^4   */
        result += pow4 * bin2;

        /* Update binomials for next k, if any */
        if (k + 1 < n) {
            seq_t num1 = (seq_t)(n - 1 - k);
            seq_t den1 = (seq_t)(k + 1);
            bin1 = bin1 * num1 / den1;

            seq_t num2 = (seq_t)(n - k);
            seq_t den2 = (seq_t)(k + 1);
            bin2 = bin2 * num2 / den2;
        }
    }

    return result;
}
