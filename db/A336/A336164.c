/*
 * A336164  a(1) = 1; if n>1, and gcd(a(n-1), n) > 1 then a(n) =
 *          a(n-1)/gcd(a(n-1), n), otherwise a(n) = a(n-1) + n - 1.
 * https://oeis.org/A336164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1,
 *   if gcd(a(n-1),n)>1 then a(n)=a(n-1)/gcd(a(n-1),n),
 *   else a(n)=a(n-1)+n-1. */
#include <stdlib.h>          /* malloc, realloc */
#include "../utils/gcd.h"    /* seq_t gcd(seq_t, seq_t) */

/* static cache: 1‑based indexing; seq_cache[i] holds a(i). */
static seq_t *seq_cache = NULL;        /* allocated array */
static size_t seq_cache_size = 0;      /* bytes allocated / sizeof(seq_t) */
static idx_t  seq_computed_up_to = 1;  /* largest n that has been computed */
static seq_t  seq_last_value  = 1;     /* a(seq_computed_up_to) */

/* Expand cache to hold indices up to n (inclusive). */
static void grow_cache(idx_t n)
{
    size_t needed = (size_t)n + 2;     /* indices 0..n inclusive */
    if (seq_cache == NULL) {
        seq_cache = (seq_t *)malloc(needed * sizeof(seq_t));
        seq_cache_size = needed;
        seq_cache[1] = 1;             /* a(1) */
    } else if (needed > seq_cache_size) {
        seq_cache = (seq_t *)realloc(seq_cache, needed * sizeof(seq_t));
        seq_cache_size = needed;
    }
}

/* Return a(n) for n ≥ 1.  Uses memoisation so that calls can occur in any
 * order.  Complexity is O(max(n)) per call, but each term beyond the
 * current maximum is computed once. */
seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* Ensure the cache can store up to n. */
    if ((size_t)n + 1 > seq_cache_size) grow_cache(n);

    /* If already computed, return the cached value. */
    if (n <= seq_computed_up_to) return seq_cache[n];

    /* Compute missing terms up to n. */
    for (idx_t i = seq_computed_up_to + 1; i <= n; ++i) {
        seq_t d = gcd(seq_last_value, (seq_t)i);
        seq_t next = (d > 1) ? (seq_last_value / d)
                            : (seq_last_value + (seq_t)(i - 1));
        seq_cache[i] = next;
        seq_last_value = next;
        seq_computed_up_to = i;
    }
    return seq_cache[n];
}
