/*
 * A317884  Number of series-reduced achiral free pure multifunctions (with
 *          empty expressions allowed) with one atom and n positions.
 * https://oeis.org/A317884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n > 1) = a(n-1) + Sum_{0 < k < n-1} a(k) * Sum_{d |
 *            (n-k-1), d < n-k-1} a(d)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 39, u64 n <= 75, u128 n <= 148,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *a_vals = NULL;     /* a(i) values, indices start at 1 */
    static seq_t *S_vals = NULL;     /* S(m) = ∑_{d | m, d < m} a(d) */
    static size_t cap = 0;           /* allocated size (>= n+1) */
    static idx_t max_computed = 0;   /* largest n already computed */

    if (n == 0)                      /* OEIS offset is 1; undefined below */
        return 0;

    /* ensure capacity for a_vals[0..n] and S_vals[0..n] */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 2) new_cap = 2;            /* at least two slots */
        new_cap = new_cap * 2;                   /* amortised linear growth */

        seq_t *tmp;
        tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        a_vals = tmp;

        tmp = (seq_t *)realloc(S_vals, new_cap * sizeof(seq_t));
        if (!tmp) { free(a_vals); return 0;}
        S_vals = tmp;

        /* initialise newly added slots to 0 */
        for (size_t i = cap; i < new_cap; ++i) {
            a_vals[i] = 0;
            S_vals[i] = 0;
        }
        cap = new_cap;

        /* if this is the very first run, set a(1)=1 and rebuild S */
        if (max_computed == 0) {
            a_vals[1] = 1;
            /* rebuild S from scratch using the current a_vals array */
            for (size_t m = 2; m < cap; ++m) {
                seq_t sum = 0;
                for (idx_t d = 1; d < (idx_t)m; ++d) {
                    if ((idx_t)m % d == 0) sum += a_vals[d];
                }
                S_vals[m] = sum;
            }
            max_computed = 1;
        } else {
            /* when capacity grows after some numbers are known,
               recompute S for all indices < cap with the existing a_vals */
            for (size_t m = 2; m < cap; ++m) {
                seq_t sum = 0;
                for (idx_t d = 1; d < (idx_t)m; ++d) {
                    if ((idx_t)m % d == 0) sum += a_vals[d];
                }
                S_vals[m] = sum;
            }
        }
    }

    /* compute missing terms up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t sum_k = 0;
            if (i > 2) {                     /* only if k-range non‑empty */
                for (idx_t k = 1; k <= i - 2; ++k)
                    sum_k += a_vals[k] * S_vals[i - k - 1];
            }
            a_vals[i] = a_vals[i - 1] + sum_k;

            /* add a[i] to S for multiples of i (excluding i itself) */
            for (idx_t m = i + i; m < (idx_t)cap; m += i)
                S_vals[m] += a_vals[i];
        }
        max_computed = n;
    }

    return a_vals[n];
}
