/*
 * A279770  Numbers n such that the sum of digits of 7n equals 14.
 * https://oeis.org/A279770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-01)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A279770: a(n) = nth positive integer m such that
 * sum of decimal digits of 7*m equals 14.  The function
 * generates terms by simple incremental search, memoising
 * all found values. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

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

static seq_t *cache = NULL;          /* array of found terms      */
static idx_t cache_len = 0;          /* number of terms cached    */
static idx_t cache_cap = 0;          /* current array capacity    */
static idx_t next_candidate = 1;     /* smallest n not yet tested */

/* Grow the cache array to accommodate more terms. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Ensure that the cache contains at least the first n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* Find the next n whose 7*n has digit sum 14. */
        while (digit_sum((seq_t)next_candidate * (seq_t)7) != (seq_t)14)
            ++next_candidate;

        /* Store the found term. */
        if (cache_len == cache_cap)
            grow_cache();

        cache[cache_len++] = next_candidate;
        ++next_candidate;   /* start next search from the following integer */
    }
}

/* Return the n-th term of the sequence (a(1) is the first term). */
extern seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1, return 0 for safety. */

    ensure_cache(n);
    return cache[n - 1];
}
