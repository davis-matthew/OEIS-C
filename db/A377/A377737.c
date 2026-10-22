/*
 * A377737  Expansion of e.g.f. (1/x) * Series_Reversion( x/(1 - log(1-2*x) /
 *          2) ).
 * https://oeis.org/A377737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} 2^(n-k) * |Stirling1(n,k)|/(n-k+1)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* maximum n expected (≈10000) */
    enum { MAX_N = 10000 };

    /* tables and state */
    static seq_t a_cache[MAX_N + 1];
    static seq_t w_prev_arr[MAX_N + 1];
    static seq_t w_curr_arr[MAX_N + 1];
    static seq_t pow2[MAX_N + 1];

    static seq_t *w_prev = w_prev_arr;
    static seq_t *w_curr = w_curr_arr;

    static idx_t computed = 0;     /* number of a values already known */
    static idx_t pow2_len = 1;     /* smallest index >= 0 not yet filled in pow2 */
    static seq_t fact = (seq_t)1;   /* (cur-1)! for current cur */

    /* initialise for n = 0 */
    if (computed == 0) {
        a_cache[0] = (seq_t)1;
        w_prev[0] = (seq_t)1;      /* W_{0,0} = 1 */
        pow2[0] = (seq_t)1;
        computed = 1;
        fact = (seq_t)1;
    }

    if (n < computed) {
        return a_cache[n];
    }

    /* grow precomputed values up to n */
    for (idx_t cur = computed; cur <= n; ++cur) {
        /* extend powers of two if needed */
        if (cur >= pow2_len) {
            for (idx_t i = pow2_len; i <= cur; ++i) {
                pow2[i] = pow2[i - 1] * (seq_t)2;
            }
            pow2_len = cur + 1;
        }

        /* update factorial: fact = cur! (starting with (cur-1)! from previous loop) */
        if (cur > 0) {
            fact = fact * (seq_t)cur;
        }

        /* compute current row of W values */
        w_curr[0] = (seq_t)0;   /* W_{cur,0} = 0 for cur > 0 */

        if (cur > 0) {
            for (idx_t k = 1; k < cur; ++k) {
                seq_t term1 = (seq_t)cur * w_prev[k - 1];
                seq_t term2 = ((seq_t)cur * (seq_t)(cur - 1) * w_prev[k]) /
                              (seq_t)(cur - k + 1);
                w_curr[k] = term1 + term2;
            }
        }

        w_curr[cur] = fact;     /* W_{cur,cur} = cur! */

        /* compute a(cur) = Σ_{k=0..cur} 2^(cur-k) * W_{cur,k} */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= cur; ++k) {
            sum = sum + w_curr[k] * pow2[cur - k];
        }
        a_cache[cur] = sum;

        /* swap rows for next iteration */
        seq_t *tmp = w_prev;
        w_prev = w_curr;
        w_curr = tmp;
    }

    computed = n + 1;
    return a_cache[n];
}
