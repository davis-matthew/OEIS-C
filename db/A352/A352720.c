/*
 * A352720  a(n) is the number of free polyominoes of width 2 and size n.
 * https://oeis.org/A352720
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 38, u64 n <= 75, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A352720: a(n) = number of free polyominoes of width 2 and size n.
   The function uses the linear recurrence obtained from the rational
   generating function:
       a(n) = a(n-1) + 3*a(n-2) - a(n-3) - 2*a(n-4) - 2*a(n-5)
              - a(n-8) + a(n-9) + a(n-10) + a(n-11)   for n ≥ 12
   with initial values:
       a(0)=0, a(1)=0, a(2)=1, a(3)=1, a(4)=4,
       a(5)=5, a(6)=12, a(7)=18, a(8)=37, a(9)=60,
       a(10)=117, a(11)=200.
   The computation is memoised in a static array so calls can be made
   in any order.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static const seq_t init_vals[12] = {
    (seq_t)0,   /* a(0) */
    (seq_t)0,   /* a(1) */
    (seq_t)1,   /* a(2) */
    (seq_t)1,   /* a(3) */
    (seq_t)4,   /* a(4) */
    (seq_t)5,   /* a(5) */
    (seq_t)12,  /* a(6) */
    (seq_t)18,  /* a(7) */
    (seq_t)37,  /* a(8) */
    (seq_t)60,  /* a(9) */
    (seq_t)117, /* a(10) */
    (seq_t)200  /* a(11) */
};

/* Static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure the cache holds values up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_len > 0 && n < cache_len)
        return;                       /* already computed */

    /* Grow the cache to at least n+1 elements. */
    idx_t new_len = cache_len ? cache_len : 12;
    while (new_len <= n)
        new_len = new_len * 2;           /* exponential growth */

    /* Reallocate or initialise. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (cache == NULL) abort();     /* allocation failed */
        for (idx_t i = 0; i < 12 && i < new_len; ++i)
            cache[i] = init_vals[i];
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) abort();       /* realloc failed */
        cache = tmp;
        /* initialise new entries if expanding into the region above 12 */
        for (idx_t i = cache_len; i < new_len; ++i)
            cache[i] = 0;
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n < 2)                         /* offset is 2, return 0 for <2 */
        return (seq_t)0;

    ensure_cache(n);                   /* make sure cache covers up to n */

    /* Return pre‑computed values directly if already present. */
    if (n < cache_len && cache[n] != 0)
        return cache[n];

    /* Compute new values from the recurrence. */
    for (idx_t i = 12; i <= n; ++i) {
        seq_t val =
            cache[i - 1]                        /* a(i-1)   */
          + (seq_t)3 * cache[i - 2]              /* 3*a(i-2) */
          - cache[i - 3]                        /* a(i-3)   */
          - (seq_t)2 * cache[i - 4]              /* 2*a(i-4) */
          - (seq_t)2 * cache[i - 5]              /* 2*a(i-5) */
          - cache[i - 8]                        /* a(i-8)   */
          + cache[i - 9]                        /* a(i-9)   */
          + cache[i -10]                        /* a(i-10)  */
          + cache[i -11];                       /* a(i-11)  */

        cache[i] = val;
    }

    return cache[n];
}
