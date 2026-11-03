/*
 * A293976  a(2n) = a(2n-1) + a(n) for n >= 1, a(2n+1) = a(2n) + 1, a(0) = 0.
 * https://oeis.org/A293976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 139, u32 n <= 1039, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 8643 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A293976 implementation.
   Recurrence:
      a(0) = 0
      a(2n)   = a(2n-1) + a(n)
      a(2n+1) = a(2n)   + 1
   The function computes a(n) for any n ≥ 0 using memoised recursion. */

#include <stdlib.h>                 /* malloc, realloc */

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Static cache for memoisation */
static seq_t *cache = NULL;   /* array of computed values */
static size_t cache_cap = 0;  /* current capacity of cache */

/* Ensure the cache can hold at least index n.  Newly allocated
   slots are initialised to 0 (the only valid value for a(n) with n>0). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n) new_cap *= 2;

    if (cache == NULL) {
        cache = seq_alloc(new_cap);          /* allocates an array of seq_t with size new_cap */
    } else {
        cache = seq_realloc(cache, cache_cap, new_cap);/* resizes the array */
    }

    /* Initialise new elements to zero */
    for (size_t i = cache_cap; i < new_cap; ++i) {
        cache[i] = 0;
    }
    cache_cap = new_cap;
}

/* Main sequence function.  It is safe to call in any order and
   for any n ≥ 0. */
seq_t a(idx_t n)
{
    /* 0 is the only value that may legitimately be zero in the cache
       (for n>0 all terms are positive). */
    if (n > 0 && (size_t)n < cache_cap && cache[(size_t)n] != 0) {
        return cache[(size_t)n];
    }

    /* Base case */
    if (n == 0) {
        return 0;
    }

    ensure_cache(n);

    seq_t result;
    if ((n & (idx_t)1) != 0) {            /* n is odd: n = 2k+1 */
        result = a(n - 1) + (seq_t)1;
    } else {                              /* n is even: n = 2k   */
        result = a(n - 1) + a(n / 2);
    }

    /* Cache the result for future calls (skip n==0 already cached) */
    cache[(size_t)n] = result;
    return result;
}
