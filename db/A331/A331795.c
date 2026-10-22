/*
 * A331795  a(n) = Sum_{k=0..n} (-n)^k * binomial(n+1,k) * binomial(n+1,k+1).
 * https://oeis.org/A331795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-n)^k * C(n+1,k) * C(n+1,k+1)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 10, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert the unsigned index to signed seq_t once */
    const seq_t n_seq = (seq_t)n;
    const seq_t n_plus_one = n_seq + 1;
    const seq_t neg_n = -n_seq;   /* (-n) used in the recurrence */

    seq_t sum   = 0;
    seq_t power = 1;              /* (-n)^k, starts with k = 0 */
    seq_t C1    = 1;              /* C(n+1,0)   */
    seq_t C2    = n_plus_one;     /* C(n+1,1)   */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term_k = (-n)^k * C(n+1,k) * C(n+1,k+1) */
        seq_t term = power;
        term *= C1;
        term *= C2;
        sum += term;

        /* stop after the last term */
        if (k == n)
            break;

        /* Prepare for k+1 */
        power *= neg_n;   /* multiply by (-n) */

        /* C1_{k+1} = C1_k * (n+1-k) / (k+1) */
        seq_t tmp = C1;
        tmp *= (n_plus_one - (seq_t)k);
        tmp /= (seq_t)(k + 1);
        C1 = tmp;

        /* C2_{k+1} = C2_k * (n-k) / (k+2) */
        tmp = C2;
        tmp *= (n_seq - (seq_t)k);
        tmp /= (seq_t)(k + 2);
        C2 = tmp;
    }

    return sum;
}
