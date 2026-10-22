/*
 * A367470  Expansion of e.g.f. 1 / (3 - 2 * exp(x))^2.
 * https://oeis.org/A367470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * (k+1)! * Stirling2(n,k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple O(n^2) DP that works for all   */
/* supported seq_t types, including big-integer builds.           */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the base case directly. */
    if (n == 0)
        return (seq_t)1;

    /* Pre-allocate arrays large enough for the maximum N. */
    /* factorial[0..n+1], pow2[0..n], S[0..n] (two copies), a[0..n]   */
    size_t N = (size_t)n;

    seq_t *fact = (seq_t *)malloc((N + 2) * sizeof(seq_t));
    seq_t *pow2 = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *S_prev = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *S_curr = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *seq = (seq_t *)malloc((N + 1) * sizeof(seq_t));

    if (!fact || !pow2 || !S_prev || !S_curr || !seq) {
        /* Allocation failed – abort. */
        free(fact);
        free(pow2);
        free(S_prev);
        free(S_curr);
        free(seq);
        exit(1);
    }

    /* Factorials: fact[i] = i!  (indices 0..n+1)  */
    fact[0] = 1;
    fact[1] = 1;
    for (idx_t i = 2; i <= n + 1; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Powers of two: pow2[i] = 2^i (indices 0..n)  */
    pow2[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        pow2[i] = pow2[i - 1] * (seq_t)2;

    /* Stirling numbers: initialize row 0 → S_prev[0] = 1 */
    S_prev[0] = 1;
    seq[0] = 1;                 /* a(0) */

    /* Build rows for n = 1 .. N */
    for (idx_t i = 1; i <= n; ++i) {
        /* Build the i‑th Stirling row in S_curr.  */
        S_curr[0] = 0;
        S_curr[i] = 1;
        for (idx_t k = 1; k < i; ++k)
            S_curr[k] = k * S_prev[k] + S_prev[k - 1];

        /* Compute a(i) using the definition. */
        seq_t sum = 0;
        for (idx_t k = 0; k <= i; ++k)
            sum += fact[k + 1] * pow2[k] * S_curr[k];
        seq[i] = sum;

        /* Prepare for the next iteration.  */
        seq_t *tmp = S_prev;
        S_prev = S_curr;
        S_curr = tmp;
    }

    /* Extract the requested term. */
    seq_t result = seq[n];

    /* Free allocation. */
    free(fact);
    free(pow2);
    free(S_prev);
    free(S_curr);
    free(seq);

    return result;
}
