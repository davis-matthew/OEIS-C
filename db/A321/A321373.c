/*
 * A321373  Array T(n,k) read by antidiagonals where the first row is
 *          (-1)^k*A140966(k) and each subsequent row is obtained by adding
 *          A001045(k) to the preceding one.
 * https://oeis.org/A321373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 42, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A321373:
   a(n) is the nth term obtained by reading the triangular array T(n,k)
   by antidiagonals, where
        T(0,k) = (-1)^k * A140966(k)
        T(n,k) = T(n-1,k) + A001045(k)
   The involved sequences have closed forms:
        A140966(k) = (5 + (-2)^k) / 3
        A001045(k) = floor((2^k + 1) / 3)
   Hence for the position (row = m - j, col = j) belonging to the
   antidiagonal m with offset j, we have
        a(n) = ((col & 1) ? -A140966[col] : A140966[col]) + (seq_t)row * A001045[col].
   The function caches computed values and expands the cache as
   required, guaranteeing an O(1) amortised cost per call. */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache      = NULL;   /* a(n) values */
static size_t cache_cap  = 0;
static size_t cache_size = 0;       /* number of initialised terms */

/* Arrays for powers and auxiliary sequences, indexed by column k */
static seq_t *pow2_arr      = NULL;
static seq_t *pow_neg2_arr  = NULL;
static seq_t *A001045_arr  = NULL;
static seq_t *A140966_arr  = NULL;
static size_t pow_cap      = 0;

/* Ensure the auxiliary arrays are built up to at least `kmax` (inclusive) */
static void ensure_arrays(size_t kmax)
{
    if (kmax < pow_cap) return;

    size_t new_cap = kmax + 1;

    /* Reallocate arrays */
    pow2_arr     = (seq_t *)realloc(pow2_arr,     new_cap * sizeof(seq_t));
    pow_neg2_arr = (seq_t *)realloc(pow_neg2_arr, new_cap * sizeof(seq_t));
    A001045_arr  = (seq_t *)realloc(A001045_arr,  new_cap * sizeof(seq_t));
    A140966_arr  = (seq_t *)realloc(A140966_arr,  new_cap * sizeof(seq_t));

    /* Build values from current_cap up to new_cap-1 */
    for (size_t i = pow_cap; i < new_cap; ++i) {
        if (i == 0) {
            pow2_arr[i]     = (seq_t)1;
            pow_neg2_arr[i] = (seq_t)1;
        } else {
            pow2_arr[i]     = pow2_arr[i-1] * ((seq_t)2);
            pow_neg2_arr[i] = pow_neg2_arr[i-1] * ((seq_t)-2);
        }
        /* A001045(k) = floor((2^k + 1) / 3) */
        A001045_arr[i] = (pow2_arr[i] + (seq_t)1) / ((seq_t)3);
        /* A140966(k) = (5 + (-2)^k) / 3 */
        A140966_arr[i] = ((seq_t)5 + pow_neg2_arr[i]) / ((seq_t)3);
    }
    pow_cap = new_cap;
}

/* Ensure cache big enough to hold indices up to n */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_size > n) return;
    /* Grow cache to at least n+1 */
    size_t new_cap = cache_cap;
    while (new_cap <= (size_t)n) new_cap = (new_cap == 0) ? 8 : new_cap * 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;

    /* Determine the maximum column index needed for new entries */
    size_t needed_max_col = 0;
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Find m such that m*(m+1)/2 <= i < (m+1)*(m+2)/2 */
        size_t m = 0;
        while ((size_t)(m+1) * (m+2) / 2 <= (size_t)i) ++m;
        size_t start = m * (m + 1) / 2;
        size_t j = (size_t)i - start;
        if (j > needed_max_col) needed_max_col = j;
    }

    ensure_arrays(needed_max_col);

    /* Compute a(n) for all new indices */
    size_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= cache_size) ++m;          /* current diag */
    size_t start_of_diag = m * (m + 1) / 2;

    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        /* Advance to correct antidiagonal if needed */
        while (i >= (m + 1) * (m + 2) / 2) {
            ++m;
            start_of_diag = m * (m + 1) / 2;
        }
        size_t j = i - start_of_diag;            /* column index */
        size_t row = m - j;                      /* row index */
        size_t col = j;

        seq_t term = ((col & 1) ? -A140966_arr[col] : A140966_arr[col])
                     + ((seq_t)row) * A001045_arr[col];
        cache[i] = term;
    }
    cache_size = (size_t)n + 1;
}

/* Public function required by the harness */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
