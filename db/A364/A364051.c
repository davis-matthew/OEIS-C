/*
 * A364051  G.f. satisfies A(x) = 1 + x*A(x)^2 / (1 + x*A(x)^5).
 * https://oeis.org/A364051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/n) * Sum_{k=0..n-1} (-1)^k * binomial(n,k) *
 *            binomial(2*n+3*k, n-1-k) a(0) = 1
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 13, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t binom_fast(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)n - (seq_t)k + (seq_t)i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    seq_t A = 1;                 /* binomial(n,0)          */
    idx_t  M = 2 * n;             /* initial M = 2n          */
    idx_t  R = n - 1;             /* initial R = n‑1         */
    seq_t  B = binom_fast(M, R);  /* binomial(2n, n-1)      */

    for (idx_t k = 0; k < n; ++k) {
        seq_t term = A * B;
        if (k & 1) sum -= term; else sum += term;

        if (k + 1 == n) break;        /* finished loop           */

        /* Update A for next k:  A = C(n,k+1)  */
        A = A * ((seq_t)(n - k))      /* (n-k)   */
            / ((seq_t)(k + 1));      /* (k+1)   */

        /* Prepare next M and R:  M = 2n+3*(k+1),  R = n-2-k   */
        M += 3;
        --R;

        /* Compute the next B value */
        B = binom_fast(M, R);
    }

    return sum / (seq_t)n;
}
