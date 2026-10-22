/*
 * A319950  a(n) = Product_{i=1..n} floor(5*i/3).
 * https://oeis.org/A319950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} floor(5*i/3)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache for the sequence values */
static seq_t *seq_cache = NULL;          /* pointer to cache array   */
static size_t  cache_cap   = 0;           /* current capacity        */
static idx_t   cache_max   = 0;           /* largest n computed      */

/* Ensure the cache can hold values up to n (inclusive) */
static void
ensure_cache_size(idx_t n)
{
    /* No need to do anything if already large enough */
    if (cache_cap >= (size_t)n) return;

    /* Grow capacity exponentially */
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < (size_t)n) new_cap <<= 1;

    /* Reallocate (or allocate on first use) */
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        /* Allocation failed – abort (undefined behaviour for sequence) */
        abort();
    }
    seq_cache = tmp;
    cache_cap = new_cap;
}

/*
 * Compute a(n) = Product_{i=1..n} floor(5*i/3)
 * The function is memoised: subsequent calls are O(1) for already
 * computed values, and O(n - cache_max) for new larger n.
 */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, but return 0 */

    /* Make sure the cache is large enough */
    ensure_cache_size(n);

    /* Initialise first term if this is the first call */
    if (cache_max == 0) {
        seq_cache[0] = 1;          /* a(1) = floor(5*1/3) = 1 */
        cache_max   = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        seq_t term = (seq_t)k * 5 / 3;              /* floor(5*k/3) */
        seq_cache[k - 1] = seq_cache[k - 2] * term; /* a(k) = a(k-1)*term */
    }

    /* Update maximum computed */
    if (n > cache_max) cache_max = n;

    return seq_cache[n - 1];
}
