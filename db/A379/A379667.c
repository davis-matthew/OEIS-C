/*
 * A379667  Number of finite multisets of positive integers with sum + product
 *          = n.
 * https://oeis.org/A379667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of finite multisets of positive integers with sum
 *            + product = n. The values are computed by enumerating all
 *            unordered factorizations of each product p, adding the
 *            contribution to every n ≥ p + sum_of_factors. This yields a fast
 *            O(n²) algorithm for n up to about 10 000
 * Range:     u8 n <= 108, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Recursive helper: enumerate unordered factorizations of rem (>=1)
   with factors >= min_f.  cur_sum is the sum of selected factors.
   The original product is p; nmax is the maximum n we are computing.
   diff[n] counts how many factorizations start at n (suffix increment). */
static void dfs(idx_t rem, idx_t min_f, seq_t cur_sum,
                idx_t p, long long *diff, idx_t nmax)
{
    if (rem == 1) {
        idx_t n_start = p + (idx_t)cur_sum;   /* smallest n for this partition  */
        if (n_start <= nmax) diff[n_start]++; /* start a suffix at n_start   */
        return;
    }
    for (idx_t f = min_f; f <= rem; ++f) {
        if (rem % f == 0) {
            dfs(rem / f, f, cur_sum + (seq_t)f, p, diff, nmax);
        }
    }
}

static seq_t *cache_vals = NULL;   /* cached results */
static idx_t   cache_size = 0;     /* highest n cached so far */

seq_t a(idx_t n)
{
    /* Handle the special case n==0 directly */
    if (n == 0) return (seq_t)0;

    /* Recompute the table if necessary */
    if (!cache_vals || n > cache_size) {
        /* Allocate space for the new table */
        seq_t *new_vals = (seq_t *)calloc(n + 1, sizeof(seq_t));
        long long *diff  = (long long *)calloc(n + 1, sizeof(long long));

        /* Enumerate all products p from 1 to n (inclusive) */
        for (idx_t p = 1; p <= n; ++p) {
            dfs(p, 2, (seq_t)0, p, diff, n);
        }

        /* Prefix sum over diff to obtain final counts */
        long long acc = 0;
        for (idx_t i = 0; i <= n; ++i) {
            acc += diff[i];
            new_vals[i] = (seq_t)acc;
        }

        /* Replace old cache with the new one */
        free(diff);
        free(cache_vals);
        cache_vals  = new_vals;
        cache_size  = n;
    }
    return cache_vals[n];
}
