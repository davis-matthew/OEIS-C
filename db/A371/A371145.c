/*
 * A371145  E.g.f. satisfies log(A(x)) = x^2*A(x)^2 * (exp(x*A(x)) - 1).
 * https://oeis.org/A371145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A371145
 * Formula: a(n) = n! * Σ_{k=0..⌊n/3⌋} (n+1)^(k-1) * S(n-2k, k) / (n-2k)!
 * where S(m, k) is Stirling number of the second kind.
 * The implementation evaluates the sum directly, computing factorials,
 * Stirling numbers and powers on demand. */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    /* maximum k needed */
    idx_t kmax = n / 3;

    /* factorial array 0..n */
    seq_t *fact = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!fact) return 0;
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* powers (n+1)^j , j = 0..kmax   (only j = k-1 used) */
    seq_t *pow_n1 = (seq_t *)calloc((size_t)kmax + 1, sizeof(seq_t));
    if (!pow_n1) { free(fact); return 0; }
    pow_n1[0] = (seq_t)1;
    seq_t base = (seq_t)(n + 1);
    for (idx_t j = 1; j <= kmax; ++j)
        pow_n1[j] = pow_n1[j - 1] * base;

    /* arrays for Stirling numbers S(m, k) */
    seq_t *S_prev = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *S_cur  = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!S_prev || !S_cur) { free(fact); free(pow_n1); free(S_prev); free(S_cur); return 0; }

    /* initial S(m,0) */
    S_prev[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        S_prev[i] = (seq_t)0;

    seq_t sum = (seq_t)0;

    for (idx_t k = 1; k <= kmax; ++k) {
        /* compute S(·, k) up to m = n-2k */
        idx_t m_limit = n - 2 * k;
        if (m_limit < k) m_limit = k;          /* safety */

        /* ensure base for recurrence */
        S_cur[k - 1] = (seq_t)0;

        for (idx_t m = k; m <= m_limit; ++m) {
            seq_t tmp = S_cur[m - 1] * (seq_t)k + S_prev[m - 1];
            S_cur[m] = tmp;
        }

        idx_t m = n - 2 * k;
        seq_t factor1 = fact[n] / fact[m];                 /* n! / (m)!   */
        seq_t part    = pow_n1[k - 1] * S_cur[m];          /* (n+1)^(k-1) * S(m,k) */
        seq_t term    = factor1 * part;
        sum += term;

        /* prepare for next k */
        seq_t *tmp = S_prev;
        S_prev = S_cur;
        S_cur  = tmp;
    }

    free(fact);
    free(pow_n1);
    free(S_prev);
    free(S_cur);
    return sum;
}
