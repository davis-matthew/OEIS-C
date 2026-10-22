/*
 * A317509  Coefficients in expansion of 1/(1 + x - 2*x^5).
 * https://oeis.org/A317509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 19, i16 n <= 39, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = -a(n-1) + 2*a(n-5),  a(0)=1,  a(n)=0 for n<0.
   Computes the nth term using memoised recurrence. */
#include <stdlib.h>

static seq_t   *cache = NULL;   /* array of computed terms */
static size_t   cap   = 0;      /* current capacity of `cache` */
static size_t   last  = 0;      /* highest index already computed */

seq_t a(idx_t n)
{
    /* Initialise cache if it does not exist yet. */
    if (cache == NULL) {
        cap   = 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * cap);
        cache[0] = 1;  /* a(0) = 1 */
        last   = 0;
    }

    /* Grow cache if the requested index is beyond current capacity. */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* If we already have the value, return it immediately. */
    if ((size_t)n <= last)
        return cache[n];

    /* Compute missing terms up to `n`. */
    /* recurrence: a(k) = -a(k-1) + 2*a(k-5)   (k >= 1) */
    for (idx_t k = (idx_t)last + 1; k <= n; ++k) {
        seq_t val = -cache[k - 1];
        if (k >= 5) {
            val += 2 * cache[k - 5];
        }
        cache[k] = val;
    }
    last = (size_t)n;
    return cache[n];
}
