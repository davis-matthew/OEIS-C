/*
 * A357381  Expansion of Product_{k>=1} 1 / (1 + x^Fibonacci(k)).
 * https://oeis.org/A357381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 23, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A357381:  coefficient a(n) of the series
    \prod_{k\ge 1} \frac{1}{1+x^{F_k}},
    where F_k is the k‑th Fibonacci number.
    Computation is performed by sequentially dividing the current
    polynomial by (1+x^{F_k}) for all Fibonacci numbers F_k ≤ n.
*/

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of already computed coefficients */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;            /* number of allocated elements */

    /* recompute if the cache is insufficient */
    if (n >= dp_size) {
        /* allocate (n+1) elements, resize if needed */
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t*)realloc(dp, new_size * sizeof(seq_t));
        if (!new_dp) abort();             /* allocation failed */
        dp = new_dp;

        /* initialise coefficients: dp[0] = 1, others 0 */
        dp[0] = 1;
        for (idx_t i = 1; i < new_size; ++i) dp[i] = 0;

        /* generate Fibonacci numbers up to n, including both 1's */
        idx_t f_prev = 1, f_cur = 1;
        /* process first 1 */
        if (f_prev <= n) {
            /* first 1 */
            for (idx_t i = f_prev; i < new_size; ++i) {
                dp[i] = dp[i] - dp[i - f_prev];
            }
            /* second 1 */
            for (idx_t i = f_cur; i < new_size; ++i) {
                dp[i] = dp[i] - dp[i - f_cur];
            }
        }
        /* generate the rest of the Fibonacci sequence */
        while (1) {
            idx_t f_next = f_prev + f_cur;
            if (f_next > n) break;
            f_prev = f_cur;
            f_cur = f_next;
            /* divide by (1 + x^{f_cur}) */
            for (idx_t i = f_cur; i < new_size; ++i) {
                dp[i] = dp[i] - dp[i - f_cur];
            }
        }

        dp_size = new_size;
    }

    return dp[n];
}
