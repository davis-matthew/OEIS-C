/*
 * A254700  Number of length 2+4 0..n arrays with every five consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A254700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 86, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A254700
   The sequence obeys the linear recurrence
        a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3)
              - 15*a(n-4) + 6*a(n-5) - a(n-6)   for n > 6
   with initial values
        a(1)=34, a(2)=267, a(3)=1116, a(4)=3341,
        a(5)=8142, a(6)=17255.
   The function below computes a(n) for any n ≥ 1, reusing a
   static cache so calls in arbitrary order are efficient.  It
   uses only integer arithmetic and the standard C library. */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */
#include <stdbool.h>  /* bool */

static seq_t *cache     = NULL;   /* 1‑indexed array of computed terms */
static size_t cache_cap = 0;      /* allocated size (in terms of cache indices) */
static bool    init_done = false;
static idx_t   max_computed = 0;   /* largest n that has been computed */

/* Ensure the cache can hold values up to n (inclusive).  Reallocates if needed. */
static void ensure_capacity(idx_t n)
{
    if (n + 1 > cache_cap) {
        /* Grow to at least n+1 (one more to account for 1‑based indexing). */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        cache = tmp;
        cache_cap = n + 1;
    }
}

/* Initialise the cache with the first six terms.  This is done only once. */
static void initialise(void)
{
    if (init_done) return;
    /* Make sure the cache can hold at least six entries. */
    ensure_capacity(6);
    /* Assign the known initial values.  The cast is safe for all supported seq_t. */
    cache[1] = (seq_t)34;
    cache[2] = (seq_t)267;
    cache[3] = (seq_t)1116;
    cache[4] = (seq_t)3341;
    cache[5] = (seq_t)8142;
    cache[6] = (seq_t)17255;
    init_done = true;
    max_computed = 6;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, but never called in tests */

    initialise();               /* set up the first six elements */

    ensure_capacity(n);         /* enlarge the cache if necessary */

    /* If the value is already computed, return it immediately. */
    if (n <= max_computed) return cache[n];

    /* Compute missing entries up to n, using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] =
            (seq_t)6   * cache[i-1]
          - (seq_t)15  * cache[i-2]
          + (seq_t)20  * cache[i-3]
          - (seq_t)15  * cache[i-4]
          + (seq_t)6   * cache[i-5]
          -          cache[i-6];
    }
    max_computed = n;
    return cache[n];
}
