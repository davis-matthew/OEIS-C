/*
 * A400345  a(0) = 1, a(1) = 0; a(2*n) = a(2*n-2) + a(n), a(2*n+1) = a(n+1) -
 *          a(n).
 * https://oeis.org/A400345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1, a(1) = 0; a(2*n) = a(2*n-2) + a(n); a(2*n+1) = a(n+1)
 *            - a(n);
 * Range:     i8 n <= 47, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation table */
static seq_t *cache = NULL;   /* array of values */
static size_t cache_size = 0; /* number of elements (max index + 1) */

/* Grow the table so that indices up to n are addressable. */
static void grow_cache(idx_t n)
{
    size_t new_size = (size_t)n + 1;
    if (new_size <= cache_size)
        return;
    /* realloc returns void* – cast to seq_t*.  In C++11 and later this
       cast is legal; the test harness does not rely on constructors
       being run (seq_t is seen as a plain integer type in the tests). */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – aborting is safe because the sequence
           must be computable for all n. */
        abort();
    }
    cache = tmp;
    cache_size = new_size;
}

/* Compute all values up to n, inclusive, using the recurrence
   and filling the cache.  Each new value is computed once. */
static void compute_up_to(idx_t n)
{
    if ((idx_t)cache_size > n) /* already done */
        return;

    size_t old_size = cache_size;
    grow_cache(n);

    /* Produce values in increasing order; all required lower indices
       are guaranteed to be present. */
    for (idx_t i = (idx_t)old_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)0;
        } else if ((i & 1u) == 0) {          /* even */
            idx_t half = i >> 1;
            cache[i] = cache[i - 2] + cache[half];
        } else {                             /* odd */
            idx_t half = (i - 1) >> 1;
            cache[i] = cache[half + 1] - cache[half];
        }
    }
}

/* Public function required by the exercise */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
