/*
 * A376123  E.g.f. A(x) satisfies A(x) = x * exp(A(x)) * (1 + 2*A(x)).
 * https://oeis.org/A376123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} 2^(n-k) * n^(k-1) * binomial(n-1,k-1) /
 *            k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0) return 0;

    /* Compute n! */
    seq_t fact_n = 1;
    for (idx_t i = 2; i <= n; ++i) fact_n *= (seq_t)i;

    /* Initialize 2^(n-1), n^(k-1), binomial(n-1,k-1), and auxiliary product */
    seq_t pow2 = 1;
    for (idx_t i = 1; i <= n-1; ++i) pow2 *= (seq_t)2;          /* pow2 = 2^(n-1) */

    seq_t pown = 1;            /* n^(k-1) */
    seq_t bin   = 1;           /* binomial(n-1,k-1)   */
    seq_t q     = fact_n;       /* holds n!/(k-1)! for the current k */
    seq_t sum   = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* n! / k!  (exact division) */
        seq_t qdiv = q / (seq_t)k;

        /* term = 2^(n-k) * n^(k-1) * binomial(n-1,k-1) * (n! / k!) */
        seq_t term = pow2 * pown;
        term = term * bin;
        term = term * qdiv;
        sum += term;

        /* Update auxiliary values for the next iteration */
        if (k < n) {
            bin = bin * (seq_t)(n - k) / (seq_t)k;   /* update binomial */
        }
        pow2 /= 2;                /* 2^(n-(k+1)) */
        pown *= (seq_t)n;           /* n^k   */
        q    = qdiv;          /* n! / k! becomes n!/(k!) for next step */
    }

    return sum;   /* a(n) */
}
