/*
 * A397383  a(n) = Sum_{k=0..n} binomial(n+k,2*k) * binomial(n+2*k,k-1).
 * https://oeis.org/A397383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k,2*k) * binomial(n+2*k,k-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t N = (seq_t)n;

    /* initialise for k = 1 */
    seq_t a_val = (N + 1) * N / 2;   /* binomial(n+1,2) */
    seq_t b_val = (seq_t)1;          /* binomial(n+2,0) */
    seq_t sum = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        sum += a_val * b_val;
        if (k == n) break;   /* terminate after the last term */

        seq_t kk = (seq_t)k;

        /* compute the next a_val = C(n+k+1,2*(k+1)) from a_val = C(n+k,2k) */
        seq_t tmpA = a_val;
        tmpA *= (N + kk + 1);
        tmpA *= (N - kk);
        seq_t denomA = (seq_t)(2 * kk + 2) * (seq_t)(2 * kk + 1);
        tmpA /= denomA;
        a_val = tmpA;

        /* compute the next b_val = C(n+2(k+1),k) from b_val = C(n+2k,k-1) */
        seq_t tmpB = b_val;
        tmpB *= (N + 2 * kk + 2);
        tmpB *= (N + 2 * kk + 1);
        seq_t denomB = kk * (N + kk + 2);
        tmpB /= denomB;
        b_val = tmpB;
    }

    return sum;
}
