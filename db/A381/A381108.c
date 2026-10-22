/*
 * A381108  Expansion of e.g.f. log(1-x)^2 * (exp(x) - 1) / (2 * (1-x)).
 * https://oeis.org/A381108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Sum_{k=0..n-1} binomial(n,k) * |Stirling1(k+1,3)|
 *   = Sum_{k=0..n-1} binomial(n,k) * c(k+1,3)
 * where c(n,3) are unsigned Stirling numbers of the first kind.
 * The function computes this sum in O(n) time using only integer
 * arithmetic on the type seq_t supplied by the build system.
 */
seq_t a(idx_t n)
{
    /* special case: n=0 */
    if (n == 0)
        return (seq_t)0;

    /* variables for unsigned Stirling numbers c(n,1), c(n,2), c(n,3) */
    seq_t c1 = (seq_t)1;   /* c(1,1) = 1 */
    seq_t c2 = (seq_t)0;   /* c(1,2) = 0 */
    seq_t c3 = (seq_t)0;   /* c(1,3) = 0 */

    seq_t sum   = (seq_t)0;  /* result */
    seq_t binom = (seq_t)1;  /* C(n,0) */

    for (idx_t m = 1; m <= n; ++m) {
        /* k = m-1: use current c3 = c(k+1,3) */
        seq_t term = binom * c3;
        sum += term;

        /* Binomial update for next k (if needed) */
        if (m < n) {
            /* binom_{k+1} = binom_k * (n - k) / (k + 1)
               where k = m-1, so (n - k) = (n - m + 1) */
            binom = binom * (seq_t)(n - m + 1) / (seq_t)m;
        }

        /* Update Stirling numbers for next n = m+1 */
        /* c(n+1,1) = n * c(n,1) */
        seq_t new_c1 = c1 * (seq_t)m;
        /* c(n+1,2) = c(n,1) + n * c(n,2) */
        seq_t new_c2 = c1 + (seq_t)m * c2;
        /* c(n+1,3) = c(n,2) + n * c(n,3) */
        seq_t new_c3 = c2 + (seq_t)m * c3;

        c1 = new_c1;
        c2 = new_c2;
        c3 = new_c3;
    }

    return sum;
}
