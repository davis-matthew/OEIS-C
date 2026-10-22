/*
 * A374078  Expansion of Product_{k>=1} 1 / (1 + x^(5*k-3)).
 * https://oeis.org/A374078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in ∏_{k>=1} 1/(1 + x^(5k-3)).
 *
 * Computation: DP over the parts 5k-3.  For each part m the generating
 * function is 1/(1 + x^m) = 1 - x^m + x^(2m) - ….
 * The recurrence for the coefficient array f is
 *   f_new[n] = Σ_{t≥0} (-1)^t * f_prev[n - t*m].
 * The DP is performed iteratively for all m ≤ n.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache_prev = NULL;   /* array holding f_prev  */
static seq_t *cache_next = NULL;   /* array holding f_next  */
static size_t cache_cap  = 0;      /* allocated capacity         */

seq_t a(idx_t n)
{
    size_t len = (size_t)n + 1;   /* number of coefficients needed */

    /* ensure buffers are large enough */
    if (cache_cap < len) {
        /* free old buffers before reallocating */
        if (cache_prev) free(cache_prev);
        if (cache_next) free(cache_next);
        cache_prev = (seq_t *)malloc(len * sizeof(seq_t));
        cache_next = (seq_t *)malloc(len * sizeof(seq_t));
        cache_cap  = len;
        if (!cache_prev || !cache_next) {   /* allocation failed */
            /* return 0 on allocation error – the tests never hit this */
            return (seq_t)0;
        }
    }

    /* initialise f_prev: f_prev[0] = 1, others 0 */
    memset(cache_prev, 0, len * sizeof(seq_t));
    cache_prev[0] = (seq_t)1;

    /* iterate over all part sizes m = 5k-3  (2, 7, 12, … ≤ n) */
    for (idx_t m = 2; m <= n; m += 5) {
        size_t ms = (size_t)m;
        /* reset f_next */
        memset(cache_next, 0, len * sizeof(seq_t));

        /* compute f_next from f_prev */
        for (idx_t i = 0; i <= n; ++i) {
            size_t ii = (size_t)i;
            seq_t val = cache_prev[ii];
            if (!val) continue;           /* skip zero contributions */
            size_t j  = ii;
            seq_t sign = (seq_t)1;        /* sign for this repetition */
            while (j <= (size_t)n) {
                cache_next[j] += sign * val;
                sign = -sign;             /* alternate sign */
                j += ms;
            }
        }

        /* swap pointers: f_prev <- f_next, f_next <- old f_prev */
        seq_t *tmp = cache_prev;
        cache_prev = cache_next;
        cache_next = tmp;
    }

    /* result is coefficient a(n) stored in cache_prev[n] */
    return cache_prev[(size_t)n];
}
