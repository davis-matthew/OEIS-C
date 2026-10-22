/*
 * A334258  a(n) = (-1)^n * exp(n) * Sum_{k>=1} (-1)^k * n^(k-1) * k^n / k!.
 * https://oeis.org/A334258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 11, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=1..n} (-1)^(n-k) * Stirling2(n,k) * n^(k-1)
 * (Stirling numbers of the second kind are computed recursively.)
 */
#include <stdlib.h>

static seq_t *a_cache = NULL;   /* memoized a(0..max_n), a(0)=0   */
static seq_t *s_prev = NULL;    /* Stirling(n-1, k) for current n-1  */
static seq_t *s_curr = NULL;    /* Stirling(n,   k) for current n    */
static idx_t  cache_max = 0;    /* highest n computed so far          */

seq_t a(idx_t n)
{
    /* Ensure memoization tables are large enough for n */
    if (n > cache_max) {
        size_t new_cap = (size_t)n + 1;   /* include index 0 */

        /* Allocate or grow a_cache */
        if (a_cache == NULL) {
            a_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        } else {
            a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        }

        /* Allocate or grow Stirling rows */
        if (s_prev == NULL || s_curr == NULL) {
            s_prev = (seq_t*)malloc(new_cap * sizeof(seq_t));
            s_curr = (seq_t*)malloc(new_cap * sizeof(seq_t));
        } else {
            s_prev = (seq_t*)realloc(s_prev, new_cap * sizeof(seq_t));
            s_curr = (seq_t*)realloc(s_curr, new_cap * sizeof(seq_t));
        }

        /* Initialise new entries to zero */
        for (size_t i = cache_max + 1; i <= n; ++i) {
            a_cache[i] = (seq_t)0;
            s_prev[i] = (seq_t)0;
            s_curr[i] = (seq_t)0;
        }

        /* Initialise base Stirling row if building from scratch */
        if (cache_max == 0) {
            s_prev[0] = (seq_t)1;          /* S(0,0) = 1 */
        }

        /* Compute rows for i = cache_max+1 .. n */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            s_curr[0] = (seq_t)0;          /* S(i,0) = 0 for i>=1 */

            /* Compute Stirling numbers for row i */
            for (idx_t k = 1; k <= i; ++k) {
                seq_t part1 = s_prev[k] * (seq_t)k;   /* k * S(i-1,k)   */
                seq_t part2 = s_prev[k-1];            /* S(i-1,k-1)     */
                s_curr[k] = part1 + part2;
            }

            /* Compute a(i) from current row */
            seq_t sum = (seq_t)0;
            seq_t pow = (seq_t)1;                           /* i^(k-1) */
            seq_t sign = ((i - 1) & 1) ? (seq_t)(-1) : (seq_t)1; /* (-1)^(i-1) */

            for (idx_t k = 1; k <= i; ++k) {
                seq_t term = s_curr[k] * pow;
                if (sign < (seq_t)0)
                    sum -= term;
                else
                    sum += term;
                sign = -sign;
                pow *= (seq_t)i;
            }

            a_cache[i] = sum;

            /* Prepare for next iteration: swap rows */
            seq_t *tmp = s_prev;
            s_prev = s_curr;
            s_curr = tmp;
        }

        cache_max = n;
    }

    return a_cache[n];
}
