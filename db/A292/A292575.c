/*
 * A292575  For 1 <= n <= 3, a(n) = n; thereafter, a(2n) = a(n) + a(n+1),
 *          a(2n-1) = a(n) + a(n-2).
 * https://oeis.org/A292575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 233, u16 n <= 9918, u32 n <= 9918, u64 n <= 9918,
 *            u128 n <= 9918, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A292575:
   a(1)=1 a(2)=2 a(3)=3;
   for n>3:
      if n even:   a(n)=a(n/2)+a(n/2+1)
      if n odd:    a(n)=a((n+1)/2)+a((n+1)/2-2)   (n>=5)
   Indexing starts at 1.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache     = NULL;  /* cache of computed values */
static bool *computed   = NULL;  /* whether cache entry is valid */
static size_t cache_cap = 0;     /* current capacity */

/* Ensure the cache can hold index n. */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        bool  *tmp_comp  = (bool*)realloc(computed, new_cap * sizeof(bool));
        if (!tmp_cache || !tmp_comp)
            exit(1);
        if (new_cap > cache_cap) {
            for (size_t i = cache_cap; i < new_cap; ++i)
                tmp_comp[i] = false;
        }
        cache     = tmp_cache;
        computed  = tmp_comp;
        cache_cap = new_cap;
    }
}

/* Recursive evaluation with memoisation. */
static seq_t eval(idx_t n)
{
    if (n <= 3)
        return (seq_t)n;
    ensure_cache(n);
    if (computed[(size_t)n])
        return cache[(size_t)n];
    seq_t res;
    if ((n & 1u) == 0) {            /* even */
        idx_t k = n >> 1;
        res = eval(k) + eval(k + 1);
    } else {                        /* odd */
        idx_t k = (n + 1) >> 1;
        res = eval(k) + eval(k - 2);
    }
    cache[(size_t)n] = res;
    computed[(size_t)n] = true;
    return res;
}

/* Public API for the sequence. */
seq_t a(idx_t n)
{
    return eval(n);
}
