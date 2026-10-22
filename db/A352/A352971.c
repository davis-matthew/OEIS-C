/*
 * A352971  Moments of the distribution of position of the first occurrence of
 *          pattern aa in a random ternary word.
 * https://oeis.org/A352971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A352971: Moments of the distribution of position of the first occurrence of pattern aa in a random ternary word.
 *
 * Recurrence derived from the exponential generating function:
 *   a(0) = 1
 *   For n ≥ 1:
 *     a(n) = 6 * Σ_{k=0}^{n-1} binom(n,k) * a(k)
 *            + 2 * Σ_{k=0}^{n-1} binom(n,k) * a(k) * 2^{n-k}
 *            + 2^n
 *
 * All operations are performed in the unsigned integer type seq_t.
 *
 * The implementation uses dynamic programming:
 *   - binom[k] holds binom(current_n, k) for the current n.
 *   - pow2[i] holds 2^i.
 *   - a[i] stores a(i) for all i ≤ n.
 *
 * Complexity: O(n^2) arithmetic operations, which is fine for n ≤ 10 000 and fits within the time limit.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate arrays */
    seq_t *a_val   = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *pow2    = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *binom   = (seq_t *)malloc((n + 1) * sizeof(seq_t));

    if (!a_val || !pow2 || !binom) {
        /* Allocation failed – return 0 as sentinel */
        return (seq_t)0;
    }

    /* Initial values */
    a_val[0] = (seq_t)1;
    pow2[0]  = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow2[i] = pow2[i - 1] * (seq_t)2;

    for (idx_t i = 0; i <= n; ++i)
        binom[i] = (seq_t)0;
    binom[0] = (seq_t)1;

    /* Main DP loop */
    for (idx_t m = 1; m <= n; ++m) {
        /* Update binomial coefficients for current m */
        for (idx_t k = m; k >= 1; --k)
            binom[k] = binom[k] + binom[k - 1];

        /* Compute the two sums */
        seq_t sum_bin = (seq_t)0;   /* Σ_{k=0}^{m-1} binom(m,k) * a(k)         */
        seq_t sum_bin2 = (seq_t)0;  /* Σ_{k=0}^{m-1} binom(m,k) * a(k) * 2^{m-k}*/

        for (idx_t k = 0; k < m; ++k) {
            seq_t prod = binom[k] * a_val[k];
            sum_bin += prod;
            sum_bin2 += prod * pow2[m - k];
        }

        /* Compute a(m) using the recurrence */
        a_val[m] = (seq_t)6 * sum_bin + (seq_t)2 * sum_bin2 + pow2[m];
    }

    seq_t result = a_val[n];

    /* Free memory */
    free(a_val);
    free(pow2);
    free(binom);

    return result;
}
