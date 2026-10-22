/*
 * A341189  Numbers that when divided by the sum of their digits leave 19 as
 *          remainder.
 * https://oeis.org/A341189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1059, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A341189: a(n) is the n‑th positive integer such that
   a(n) % digit_sum(a(n)) == 19.  (digit_sum uses base‑10).  The
   implementation caches previously found terms so that calls in any
   order are fast. */

#include <stddef.h>      /* size_t */
#include <stdint.h>      /* uint64_t */
#include <stdlib.h>      /* malloc, realloc, free */
#include <stdbool.h>     /* bool */
#include "../utils/digit_sum.h"

/* Cache the already found terms.  The cache is grown only when
   necessary.  Cur_val stores the last tested integer (starts
   before the first term). */
static seq_t *cache = NULL;      /* array of cached terms */
static size_t cache_len = 0;     /* number of terms cached (<= cache_cap) */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static seq_t cur_val = (seq_t)298;   /* start just before the first term */

/* Helper that ensures the cache has room for at least 'n' terms.
   The cache grows exponentially to keep amortised O(1) growth
   time. */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n <= cache_cap)
        return;
    size_t new_cap = 32;          /* initial capacity */
    while (new_cap < (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)                    /* allocation failure – abort */
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the n‑th term of A341189.
   n >= 1.  For n <= 0 the function returns 0 (undefined). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    /* make sure enough space in the cache */
    ensure_cache_capacity(n);

    /* generate terms until we have n of them */
    while (cache_len < (size_t)n) {
        seq_t sum;
        do {
            cur_val++;                 /* next integer to test */
            sum = digit_sum(cur_val);  /* digit sum in base 10 */
        } while (!(sum > (seq_t)19 && cur_val % sum == (seq_t)19));
        cache[cache_len++] = cur_val;   /* store found term */
    }
    return cache[n - 1];
}
