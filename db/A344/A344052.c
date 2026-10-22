/*
 * A344052  a(n) = Sum_{k=0..n} (-1)^(n-k)*binomial(n, k)*E1(n, k).
 * https://oeis.org/A344052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k)*binomial(n, k)*E1(n, k)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here E1(n,k) are the standard first‑order Eulerian numbers. */

#include <stdlib.h>

/* static state for memoisation */
static seq_t *a_cache = NULL;            /* cache of computed a(n) values      */
static size_t cache_capacity = 0;        /* current allocated size of a_cache  */
static idx_t max_cached = 0;             /* highest n that has been computed  */

static seq_t *prev_row = NULL;           /* Eulerian numbers for current n    */
static seq_t *cur_row = NULL;            /* temporary array for next row      */
static size_t row_capacity = 0;          /* current allocated capacity of rows*/

static void initialise(void) {
    /* initialise caches for n = 0 */
    cache_capacity = 1;
    a_cache = (seq_t*)malloc(cache_capacity * sizeof(seq_t));
    a_cache[0] = (seq_t)1;               /* a(0) = 1 */

    row_capacity = 1;
    prev_row = (seq_t*)malloc(row_capacity * sizeof(seq_t));
    prev_row[0] = (seq_t)1;              /* Eulerian numbers for n = 0: A(0,0) = 1 */
    cur_row = NULL;                      /* will be allocated on first use    */
}

/* compute a(n) for all n up to and including n, caching the results */
static void compute_up_to(idx_t n) {
    if (n <= max_cached) return;    /* already computed */

    /* extend caches if necessary */
    if (cache_capacity <= (size_t)n) {
        cache_capacity = n + 1;
        a_cache = (seq_t*)realloc(a_cache, cache_capacity * sizeof(seq_t));
    }

    /* generate Eulerian numbers row by row */
    for (idx_t curr = max_cached + 1; curr <= n; ++curr) {
        /* ensure we have enough space for the new row */
        if (row_capacity < (size_t)(curr + 1)) {
            prev_row = (seq_t*)realloc(prev_row, (curr + 1) * sizeof(seq_t));
            cur_row  = (seq_t*)realloc(cur_row,  (curr + 1) * sizeof(seq_t));
            row_capacity = curr + 1;
        }

        /* compute the Eulerian numbers for n = curr */
        for (idx_t k = 0; k <= curr - 1; ++k) {
            seq_t term1 = (seq_t)(k + 1) * prev_row[k];
            seq_t term2 = 0;
            if (k > 0)
                term2 = (seq_t)(curr - k) * prev_row[k - 1];
            cur_row[k] = term1 + term2;
        }
        cur_row[curr] = 0;  /* A(curr, curr) = 0 */

        /* compute a_curr from the current Eulerian row */
        seq_t a_curr = (seq_t)0;
        seq_t binom = (seq_t)1;           /* binomial(curr, 0) */
        for (idx_t k = 0; k <= curr - 1; ++k) {
            seq_t term = cur_row[k] * binom;
            if (((curr - k) & 1) == 0)    /* even exponent -> + */
                a_curr += term;
            else
                a_curr -= term;

            /* update binomial coefficient to binomial(curr, k+1) */
            if (k < curr - 1)
                binom = (binom * (seq_t)(curr - k)) / (seq_t)(k + 1);
        }

        a_cache[curr] = a_curr;

        /* swap prev_row and cur_row for next iteration */
        seq_t *tmp = prev_row;
        prev_row = cur_row;
        cur_row = tmp;
    }

    max_cached = n;
}

/* public interface: returns a(n) */
seq_t a(idx_t n) {
    if (a_cache == NULL) initialise();
    compute_up_to(n);
    return a_cache[n];
}
