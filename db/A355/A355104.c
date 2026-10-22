/*
 * A355104  E.g.f. A(x) satisfies A(x) = 1 + 2 * x * A(log(1+x)).
 * https://oeis.org/A355104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; for n > 0, a(n) = 2*n*Sum_{k=0}^{n-1} Stirling1(n-1,k)
 *            * a(k)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 10, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Static cache for computed terms and the latest Stirling row */
static seq_t *a_cache   = NULL;   /* a_cache[i] = a(i) for 0 <= i <= computed_N */
static seq_t *s_prev    = NULL;   /* Stirling row for size computed_N: s(n,k) for 0 <= k <= n */
static idx_t  computed_N = 0;     /* maximum n for which a_cache and s_prev are valid */

/* Compute the n-th term of A355104 */
seq_t a(idx_t n)
{
    /* Initialise cache for n = 0 */
    if (a_cache == NULL) {
        a_cache = seq_alloc_new_array(1);
        a_cache[0] = (seq_t)1;

        s_prev = seq_alloc_new_array(1);
        s_prev[0] = (seq_t)1;        /* s(0,0) = 1 */
        computed_N = 0;
    }

    /* Extend the cache up to n if necessary */
    if (n > computed_N) {
        /* Allocate a larger array for a_cache if needed */
        if (n > computed_N) {
            seq_t *new_a = seq_alloc_new_array((size_t)(n + 1));
            for (idx_t i = 0; i <= computed_N; ++i) new_a[i] = a_cache[i];
            seq_free_array(a_cache);
            a_cache = new_a;
        }

        /* Iteratively compute terms from computed_N+1 up to n */
        for (idx_t m = computed_N + 1; m <= n; ++m) {
            /* Compute S = sum_{k=0}^{m-1} s_prev[k] * a_cache[k] */
            seq_t S = (seq_t)0;
            for (idx_t k = 0; k < m; ++k) {
                S += s_prev[k] * a_cache[k];
            }

            /* a(m) = 2 * m * S */
            seq_t tn = (seq_t)m;
            a_cache[m] = (seq_t)2 * tn * S;

            /* Compute the next Stirling row s(m,k) for 0 <= k <= m */
            seq_t *new_row = seq_alloc_new_array(m + 1);
            new_row[0] = (seq_t)0;          /* s(m,0) = 0 for m > 0 */
            for (idx_t k = 1; k < m; ++k) {
                /* s(m,k) = s(m-1,k-1) - (m-1) * s(m-1,k) */
                new_row[k] = s_prev[k - 1] - (seq_t)(m - 1) * s_prev[k];
            }
            new_row[m] = (seq_t)1;          /* s(m,m) = 1 */

            /* Replace old Stirling row */
            seq_free_array(s_prev);
            s_prev = new_row;
        }

        computed_N = n;
    }

    return a_cache[n];
}
