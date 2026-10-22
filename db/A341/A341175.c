/*
 * A341175  Numbers that when divided by the sum of their digits leave 8 as
 *          remainder.
 * https://oeis.org/A341175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 2129, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that n mod sum_of_digits(n) == 8.
   Generation by simple incremental search with memoisation. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

/* Cache for found terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static seq_t last_candidate = 0;

static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();   /* allocation failed */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* not used, but safe */

    /* generate until we have at least n terms */
    while (cache_size < n) {
        last_candidate++;                     /* next candidate */
        seq_t s = digit_sum(last_candidate);  /* sum of digits */
        /* remainder 8 only possible if s > 8 */
        if (s > (seq_t)8 && (last_candidate % s) == (seq_t)8) {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = last_candidate;
        }
    }
    return cache[n - 1];
}
