/*
 * A361885  a(n) = (1/n) * Sum_{k = 0..2*n} (n+2*k) * binomial(n+k-1,k)^3.
 * https://oeis.org/A361885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/n) * Sum_{k = 0..2*n} (n+2*k) * binomial(n+k-1,k)^3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t b = 1;            /* binomial(n-1,0) */
    seq_t n_seq = (seq_t)n;
    idx_t limit = 2 * n;

    for (idx_t k = 0; k <= limit; ++k) {
        seq_t term_coeff = n_seq + (seq_t)(2 * k);      /* n + 2*k */
        seq_t temp = b * b;                            /* b^2 */
        seq_t b3   = temp * b;                         /* b^3 */
        sum += term_coeff * b3;                        /* accumulate */

        if (k < limit) {
            seq_t num = n_seq + (seq_t)k;               /* n + k   */
            seq_t den = (seq_t)(k + 1);                 /* k + 1   */
            b = b * num / den;                         /* update b */
        }
    }
    return sum / n_seq;    /* final division by n */
}
