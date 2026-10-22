/*
 * A350290  a(n) = Sum_{k=0..n} (-1)^(n - k) * binomial(n, k) * binomial(n + k
 *          - 1, n - k).
 * https://oeis.org/A350290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n - k) * binomial(n, k) * binomial(n +
 *            k - 1, n - k)
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 18, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=1, a(1)=1, a(2)=-3, a(3)=-2, a(4)=21, … */

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;          /* running total */
    seq_t a_k = 1;          /* C(n,0) */
    seq_t b_k = 0;          /* C(n-1, n) = 0 for n>0 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = 0;

        if (k > 0) {
            if (k == 1) {
                /* C(n,1) = n */
                b_k = (seq_t)n;
            } else {
                /* Update b_k using
                   C(n+k-1, n-k) = C(n+k-2, n-k+1) *
                                   (n+k-1)*(n-k+1) / ((2k-1)*(2k-2))  */
                idx_t kk = k - 1; /* previous k value */
                seq_t num = (seq_t)(n + kk) * (seq_t)(n - kk);
                seq_t den = (seq_t)(2 * kk + 1) * (seq_t)(2 * kk);
                b_k = b_k * num / den;   /* exact division */
            }
            term = a_k * b_k;
        }

        /* add or subtract the term depending on parity of (n-k) */
        if (((n - k) & 1ULL) == 0)
            sum += term;
        else
            sum -= term;

        /* update a_k for next iteration: C(n,k+1) = C(n,k)*(n-k)/(k+1) */
        if (k < n) {
            seq_t num = (seq_t)(n - k);
            seq_t den = (seq_t)(k + 1);
            a_k = a_k * num / den;   /* exact division */
        }
    }
    return sum;
}
