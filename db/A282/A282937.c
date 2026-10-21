/*
 * A282937  a(n) = A000728(5*n).
 * https://oeis.org/A282937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; for n>0, a(n)=-(1/n)*sum_{k=1..n} sigma(5*k)*a(n-k)
 *            (sigma=divisor–sum)
 * Range:     i8 n <= 6, i16 n <= 118, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, calloc, realloc, free, abort */

static seq_t *a_cache   = NULL;   /* a_cache[0..a_size]             */
static idx_t  a_size    = 0;      /* highest index computed          */
static seq_t *sigma_arr = NULL;   /* sigma_arr[0..sigma_size]        */
static idx_t  sigma_size= 0;      /* highest index available         */

/* (Re)compute the divisor sums σ(m) for 0 ≤ m ≤ limit   */
static void recompute_sigma(idx_t limit)
{
    if (sigma_arr) {
        free(sigma_arr);
        sigma_arr = NULL;
    }

    sigma_arr = (seq_t*)calloc((size_t)(limit + 1), sizeof(seq_t));
    if (!sigma_arr) abort();

    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i)
            sigma_arr[j] += (seq_t)i;
    }
    sigma_size = limit;
}

/* Compute the n‑th term of the sequence */
seq_t a(idx_t n)
{
    /* Initialise the caches on first call */
    if (!a_cache) {
        a_cache = (seq_t*)malloc(sizeof(seq_t));
        if (!a_cache) abort();
        a_cache[0] = (seq_t)1;      /* a(0) = 1 */
        a_size   = 0;
    }

    /* Extend the caches if required */
    if (n > a_size) {
        /* Ensure we have σ(m) for m = 5·k, 1 ≤ k ≤ n */
        idx_t sigma_needed = 5 * n;
        if (sigma_needed > sigma_size)
            recompute_sigma(sigma_needed);

        /* Grow the a_cache to at least n */
        seq_t *new_cache = (seq_t*)realloc(a_cache,
                                           (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) abort();
        a_cache = new_cache;

        /* Compute the missing sequence values */
        for (idx_t m = a_size + 1; m <= n; ++m) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= m; ++k)
                sum += sigma_arr[5 * k] * a_cache[m - k];
            a_cache[m] = -(sum / (seq_t)m);   /* division is exact */
        }
        a_size = n;
    }

    return a_cache[n];
}
