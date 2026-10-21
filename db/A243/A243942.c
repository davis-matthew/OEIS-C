/*
 * A243942  O.g.f.: Sum_{n>=0} n^(2*n) * x^n / (1 - n*x)^n * exp( -n^2*x / (1
 *          - n*x) ) / n!.
 * https://oeis.org/A243942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} C(n-1, k-1) * Stirling2(n+k, k), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t max_k = n;

    /* Allocate two rows for the Stirling triangle, plus a column for the
       needed diagonal entries.  All arrays are initialized to zero.      */
    seq_t *prev = (seq_t*)calloc(max_k + 1, sizeof(seq_t));
    seq_t *curr = (seq_t*)calloc(max_k + 1, sizeof(seq_t));
    seq_t *diag = (seq_t*)calloc(max_k + 1, sizeof(seq_t));
    if (!prev || !curr || !diag) {
        /* Allocation failure – in a real system we would handle it.
           Here we simply return 0.                                   */
        free(prev); free(curr); free(diag);
        return (seq_t)0;
    }

    /* Base row (m = 0): S(0,0) = 1; all other entries are 0.           */
    prev[0] = (seq_t)1;

    /* Build Stirling numbers up to m = 2*n.  While we are doing this
       we keep track of the needed diagonal elements S(n+k, k).         */
    for (idx_t m = 1; m <= 2*n; ++m) {
        idx_t max_k_this = (m <= max_k) ? m : max_k;

        curr[0] = (seq_t)0;                     /* S(m,0) = 0 */

        for (idx_t k = 1; k <= max_k_this; ++k) {
            /* curr[k] = k * prev[k] + prev[k-1]  (Stirling recurrence) */
            seq_t t = prev[k];
            if (k != 0) t = t * (seq_t)k;
            t = t + prev[k-1];
            curr[k] = t;
        }

        /* Zero out entries that are beyond the current maximum k.   */
        for (idx_t k = max_k_this + 1; k <= max_k; ++k)
            curr[k] = (seq_t)0;

        /* Capture the diagonal value when it appears.                */
        if (m > n) {
            idx_t k = m - n;             /* because m = n + k          */
            if (k >= 1 && k <= max_k)
                diag[k] = curr[k];
        }

        /* Prepare for next iteration.                                 */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Compute the sum of binomial coefficients times diagonal values.*/
    seq_t result = (seq_t)0;
    seq_t bin = (seq_t)1;   /* C(n-1,0) */

    for (idx_t k = 1; k <= max_k; ++k) {
        result = result + diag[k] * bin;

        /* Update bin to C(n-1, k) for next iteration.               */
        seq_t num = (seq_t)(n - k);
        bin = bin * num / (seq_t)k;
    }

    free(prev);
    free(curr);
    free(diag);

    return result;
}
