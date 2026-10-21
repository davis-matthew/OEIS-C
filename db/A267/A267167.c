/*
 * A267167  Growth series for affine Coxeter group B_4.
 * https://oeis.org/A267167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 1917, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A267167 – Growth series for affine Coxeter group B_4.
   Recurrence (for n ≥ 15):
       a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + 2*a(n-7)
              - 2*a(n-8) + a(n-9) - a(n-12) + 2*a(n-13) - a(n-14)
   Initial values:
       a(0)=1   a(1)=5   a(2)=14   a(3)=31   a(4)=59
       a(5)=101 a(6)=161 a(7)=243 a(8)=351 a(9)=488
       a(10)=658 a(11)=865 a(12)=1112 a(13)=1403 a(14)=1741
*/

#include <stddef.h>                 /* size_t */
#include "../utils/alloc_seq.h"    /* alloc_seq(size_t) -> seq_t*   */
#include "../utils/grow_seq.h"     /* grow_seq(seq_t **, size_t *, size_t) */

static seq_t *cache      = NULL;   /* stored values a(0)…a(max)               */
static size_t cache_count = 0;     /* number of values actually computed      */
static size_t cache_cap   = 0;     /* allocated capacity (elements)           */

/* Initialise cache with the first 15 terms */
static void init_cache(void)
{
    if (cache != NULL) return;                 /* already initialised */

    cache_cap      = 15;
    cache          = alloc_seq(cache_cap);     /* alloc_seq handles seq_t construction */

    /* the first 15 terms – overflows are harmless for small seq_t
       (only the first 8 are used in the recurrence for larger seq_t).  */
    static const seq_t init_vals[15] = {
        1, 5, 14, 31, 59, 101, 161, 243, 351,
        488, 658, 865, 1112, 1403, 1741
    };
    for (size_t i = 0; i < 15; ++i) cache[i] = init_vals[i];
    cache_count = 15;
}

/* Ensure the cache can hold the requested index (size_t need) */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    /* grow_seq will increase cache_cap to at least 'need' */
    grow_seq(&cache, &cache_cap, need);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_cache();

    /* convert idx_t to size_t; n is guaranteed ≤ 10000 in tests */
    size_t idx = (size_t)n;

    /* If the term is already cached, return it immediately */
    if (idx < cache_count) return cache[idx];

    /* Ensure the cache can hold up to n */
    grow_cache(idx + 1);

    /* Compute missing terms up to n using the recurrence */
    for (size_t i = cache_count; i <= idx; ++i) {
        seq_t val =
            2 * cache[i - 1]        /* 2*a(i-1)            */
          - cache[i - 2]             /* -a(i-2)             */
          + cache[i - 5]             /* +a(i-5)             */
          - 2 * cache[i - 6]         /* -2*a(i-6)           */
          + 2 * cache[i - 7]         /* +2*a(i-7)           */
          - 2 * cache[i - 8]         /* -2*a(i-8)           */
          + cache[i - 9]             /* +a(i-9)             */
          - cache[i -12]             /* -a(i-12)            */
          + 2 * cache[i -13]         /* +2*a(i-13)          */
          - cache[i -14];             /* -a(i-14)            */

        cache[i] = val;
    }

    cache_count = idx + 1;
    return cache[idx];
}
