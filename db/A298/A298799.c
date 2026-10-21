/*
 * A298799  Expansion of (1-27*x)^(-1/9).
 * https://oeis.org/A298799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   grow_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A298799: expansion of (1-27*x)^(-1/9)

   Recurrence: a(0) = 1
               a(n) = ((27*n - 24) / n) * a(n-1)   for n > 0
   (Derived from n*a(n) + 3*(-9*n+8)*a(n-1) = 0)

   The implementation memoises results so that each distinct n is
   computed only once, independent of call order. */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/grow_seq_array.h"   /* void grow_seq_array(seq_t **, size_t *, size_t) */
#include "../utils/seq_alloc_new_array.h" /* seq_t *seq_alloc_new_array(size_t) */

static seq_t *cache = NULL;   /* memoised sequence values */
static size_t cache_cap = 0;   /* current allocated capacity */
static idx_t max_computed = 0; /* highest n that has been computed */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Handle the base case */
    if (n == 0) {
        /* Ensure that a(0) is stored */
        if (cache == NULL) {
            cache = seq_alloc_new_array(1);
            cache_cap = 1;
            cache[0] = (seq_t)1;
            max_computed = 0;
        } else if (cache_cap == 0) {
            cache_cap = 1;
            cache = seq_alloc_new_array(1);
            cache[0] = (seq_t)1;
            max_computed = 0;
        }
        return cache[0];
    }

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache = seq_alloc_new_array(1);
        cache_cap = 1;
        cache[0] = (seq_t)1;
        max_computed = 0;
    }

    /* Grow cache if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        grow_seq_array(&cache, &cache_cap, new_cap);
    }

    /* Compute missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t num = (seq_t)27 * (seq_t)i - (seq_t)24;  /* 27*i - 24 */
        cache[i] = (cache[i - 1] * num) / (seq_t)i;     /* exact division */
    }
    max_computed = n;

    return cache[n];
}
