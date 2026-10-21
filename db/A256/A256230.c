/*
 * A256230  Numbers n such that neither n nor 2*n is representable as x*y+x+y,
 *          where x>=y>1.
 * https://oeis.org/A256230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A256230: numbers n such that neither n nor 2*n is representable
   as x*y + x + y with x ≥ y > 1.  The function searches incrementally
   and caches the terms. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache          = NULL;   /* cached terms */
static size_t cache_cap      = 0;      /* capacity of cache */
static size_t cache_len      = 0;      /* number of terms stored */
static idx_t  next_candidate = 0;      /* next m to test */

static void cache_grow(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return true if m can be written as x*y+x+y with x≥y>1. */
static bool is_representable(seq_t m)
{
    seq_t N = m + (seq_t)1;          /* N = m+1 = (x+1)(y+1) */
    if (N < (seq_t)9) return false; /* smallest representable value is 8 */
    for (seq_t d = 3; d <= N / d; ++d)
    {
        if (N % d == (seq_t)0 && N / d >= (seq_t)3)
            return true;            /* factors a,b ≥ 3 found */
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for incorrect calls */
    while (cache_len < (size_t)n)
    {
        seq_t m      = (seq_t)next_candidate;
        seq_t two_m  = m + m;             /* 2*m */
        if (!is_representable(m) && !is_representable(two_m))
        {
            cache_grow(cache_len + 1);
            cache[cache_len++] = m;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
