/*
 * A362440  Aliquot sequence starting at 841.
 * https://oeis.org/A362440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: full, pass, 17 terms through n = 17 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 841; a(n) = sum of proper divisors of a(n‑1) for n > 1 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

/* Sum of proper divisors of x (x > 1).  For x <= 1 the sum is 0. */
static seq_t sum_proper(seq_t x)
{
    if (x <= (seq_t)1) return (seq_t)0;
    seq_t sum = (seq_t)1;                 /* 1 is always a proper divisor of x > 1 */
    seq_t sq  = floor_sqrt(x);
    for (idx_t i = 2; (seq_t)i <= sq; ++i)
    {
        if (x % (seq_t)i == (seq_t)0)
        {
            seq_t d2 = x / (seq_t)i;
            if (d2 == (seq_t)i)         /* perfect square */
                sum += (seq_t)i;
            else
                sum += (seq_t)i + d2;
        }
    }
    return sum;
}

/* Static cache for memoization */
static seq_t *cache  = NULL;   /* cache[i] holds a(i) for i >= 1 */
static size_t  cap    = 0;     /* allocated capacity (in elements) */
static size_t  len    = 0;     /* number of terms currently stored */

/* Ensure the cache has room for indices up to needed-1. */
static void ensure_capacity(size_t needed)
{
    if (cap >= needed) return;
    size_t newcap = cap ? cap * 2U : 8U;
    while (newcap < needed) newcap <<= 1U;
    seq_t *newptr = seq_alloc_new_array(newcap);
    for (size_t i = 1; i <= len; ++i) newptr[i] = cache[i];
    seq_free_array(cache);
    cache = newptr;
    cap   = newcap;
}

seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by the sequence definition. */
    ensure_capacity((size_t)n + 1U);
    if (len == 0)
    {
        cache[1] = (seq_t)841;  /* a(1) = 841 */
        len = 1;
    }
    while (len < (size_t)n)
    {
        seq_t prev = cache[len];
        seq_t next = sum_proper(prev);
        ++len;
        cache[len] = next;
    }
    return cache[(size_t)n];
}
