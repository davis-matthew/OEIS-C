/*
 * A331337  L.g.f.: log(Sum_{k>=0} k! * x^(k*(k + 1)/2) / Product_{j=1..k} (1
 *          - x^j)).
 * https://oeis.org/A331337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n * A032020(n) - Sum_{k=1}^{n-1} A032020(k) * a(n-k)
 * Range:     i8 n <= 9, i16 n <= 22, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A032020(n) are the coefficients of the series
   S(x) = Sum_{k>=0} k! * x^{k(k+1)/2} / Prod_{j=1..k} (1 - x^j).   */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h" /* seq_t *alloc_seq_array(size_t n) */
#include "../utils/free_seq_array.h"  /* void free_seq_array(seq_t *p) */

/* Forward declaration */
static void compute_to_n(idx_t N, seq_t **g_out, seq_t **a_out);

/* Cache for memoisation */
static idx_t cached_up_to = 0;
static seq_t *cached_g = NULL;
static seq_t *cached_a = NULL;

/* Compute the nth term of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* not used, but safe */

    if (n > cached_up_to) {
        /* allocate new arrays for the required range */
        seq_t *new_g = NULL;
        seq_t *new_a = NULL;
        compute_to_n(n, &new_g, &new_a);

        /* free old cached arrays */
        if (cached_g) free_seq_array(cached_g);
        if (cached_a) free_seq_array(cached_a);

        cached_g = new_g;
        cached_a = new_a;
        cached_up_to = n;
    }
    return cached_a[n];
}

/* Compute all coefficients g[0..N] and a[1..N] */
static void compute_to_n(idx_t N, seq_t **g_out, seq_t **a_out)
{
    /* Allocate result arrays */
    seq_t *g = alloc_seq_array(N + 1);
    seq_t *a = alloc_seq_array(N + 1);
    for (idx_t i = 0; i <= N; ++i) g[i] = 0, a[i] = 0;

    /* Temporary arrays for partition DP */
    seq_t *prev = alloc_seq_array(N + 1);
    seq_t *curr = alloc_seq_array(N + 1);
    for (idx_t i = 0; i <= N; ++i) prev[i] = 0;
    prev[0] = 1;                         /* p_0(0) = 1 */

    seq_t fact = (seq_t)1;                /* k! with k starting at 0 */

    for (idx_t k = 0;; ++k) {
        idx_t shift = k * (k + 1) / 2;
        if (shift > N) break;

        /* copy prev into curr: curr = p_{k-1} */
        for (idx_t i = 0; i <= N; ++i) curr[i] = prev[i];

        /* For k>0, apply recurrence: p_k(n) = p_k(n-k) + p_{k-1}(n) */
        if (k > 0) {
            for (idx_t i = k; i <= N; ++i) {
                curr[i] += curr[i - k];
            }
        }

        /* Add contribution of this k to g */
        for (idx_t i = shift; i <= N; ++i) {
            g[i] += fact * curr[i - shift];
        }

        /* Prepare for next iteration */
        fact *= (k + 1);                 /* (k+1)! for next k */
        /* swap prev and curr */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    free_seq_array(prev);
    free_seq_array(curr);

    /* Compute a[1..N] using the recurrence */
    a[0] = 0;
    for (idx_t n = 1; n <= N; ++n) {
        a[n] = (seq_t)n * g[n];
        for (idx_t k = 1; k < n; ++k) {
            a[n] -= g[k] * a[n - k];
        }
    }

    *g_out = g;
    *a_out = a;
}
