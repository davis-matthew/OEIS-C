/*
 * A376043  a(1)=1; thereafter a(n) = smallest positive integer such that
 *          Sum_{i=2..n} a(i-1)/a(i) is less than 1.
 * https://oeis.org/A376043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376043:
   a(1) = 1;
   For n > 1: a(n) = 1 + a(n-1) * ∏_{i=1}^{n-1} a(i). */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

static seq_t *cache     = NULL;   /* cached terms  */
static size_t cache_sz  = 0;      /* number of terms already computed */
static size_t cache_cap = 0;      /* size of allocated array    */
static seq_t prod       = 0;      /* product of all computed terms */

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined at 0 */
        return 0;

    size_t target = (size_t)n;   /* work with unsigned indices */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache     = alloc_seq_array(1);   /* allocate one element */
        cache_cap = 1;
        cache[0]  = (seq_t)1;            /* a(1) = 1 */
        prod      = (seq_t)1;            /* product of first term */
        cache_sz  = 1;
    }

    /* grow the cache if necessary */
    if (cache_cap < target)
        grow_seq_array(&cache, &cache_cap, target);

    /* compute any missing terms up to n */
    for (size_t i = cache_sz; i < target; ++i) {
        seq_t prev = cache[i - 1];        /* a(i) */
        seq_t next = prev * prod + 1;     /* a(i+1) */
        cache[i] = next;
        prod = prod * next;               /* update product of all terms */
    }

    cache_sz = target;
    return cache[target - 1];             /* return a(n) */
}
