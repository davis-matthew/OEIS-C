/*
 * A303404  a(0) = 0, a(1) = 1; for n >= 1, a(2*n) = a(2*n-1) - 2*a(n),
 *          a(2*n+1) = n - 2*a(n).
 * https://oeis.org/A303404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   not stated
 * Range:     i8 n <= 49, i16 n <= 1571, i32 n <= 8808, i64 n <= 8808,
 *            i128 n <= 8808, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 0, a(1) = 1;
 * For n ≥ 1,
 *   a(2*n)   = a(2*n-1) - 2*a(n),
 *   a(2*n+1) = n - 2*a(n)
 */

#include <stdlib.h>
#include <stdint.h>  /* for uint64_t if needed */

/* Allocate a static cache for sequence values */
static seq_t *seq_cache = NULL;       /* array of computed a(n) */
static size_t  cache_cap = 0;         /* current capacity of seq_cache */
static size_t  max_computed = (size_t)-1; /* highest index that has been computed */

/* Ensure seq_cache contains all values up to n, computing them if necessary */
static void ensure_cache(idx_t n)
{
    /* Lazy initialization: allocate minimal array and set a(0)=0, a(1)=1 */
    if (seq_cache == NULL) {
        cache_cap = 2;
        seq_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        seq_cache[0] = 0;
        seq_cache[1] = 1;
        max_computed = 1;
    }

    /* Nothing to do if n already covered */
    if ((size_t)n <= max_computed)
        return;

    /* Expand capacity to be at least n+1, using doubling strategy */
    size_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 2;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;

    /* Compute missing entries sequentially */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        if (i == 0) {
            seq_cache[i] = 0;          /* already set, but harmless */
        } else if (i == 1) {
            seq_cache[i] = 1;          /* already set */
        } else if ((i & 1) == 0) {     /* even index: a(2*n) */
            idx_t k = (idx_t)(i >> 1); /* k = n in a(2*n) */
            seq_cache[i] = seq_cache[i - 1] - (seq_t)2 * seq_cache[k];
        } else {                      /* odd index: a(2*n+1) */
            idx_t k = (idx_t)((i - 1) >> 1); /* k = n in a(2*n+1) */
            seq_cache[i] = (seq_t)k - (seq_t)2 * seq_cache[k];
        }
    }
    max_computed = (size_t)n;
}

/* Public function implementing the sequence */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[(size_t)n];
}
