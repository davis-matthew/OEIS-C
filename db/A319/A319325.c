/*
 * A319325  a(n) is the number of row convex polyglasses (polyiamonds which
 *          need only touch at corners) with n cells.
 * https://oeis.org/A319325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7*a(n-1) - 9*a(n-2) - a(n-3) + 9*a(n-4) - 3*a(n-5)
   for n >= 6, with initial values
   a(1)=2, a(2)=10, a(3)=52, a(4)=276, a(5)=1470.
   The implementation memoises results, expanding the cache as needed. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_nmax = 0;   /* largest n that has been computed (1‑based) */
static size_t cache_cap = 0;   /* allocated array capacity (in entries) */

/* Grow the cache to hold at least new_cap entries (n‑based). */
static void cache_expand(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                       /* allocation failure – terminate */
    if (cache) {
        for (idx_t i = 0; i < cache_nmax; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = (size_t)new_cap;
}

/* Initialise the cache with the first five terms. */
static void cache_init(void)
{
    if (cache_nmax > 0)
        return;
    cache_expand(16);                  /* start with room for up to 16 terms */
    cache[0] = (seq_t)2;
    cache[1] = (seq_t)10;
    cache[2] = (seq_t)52;
    cache[3] = (seq_t)276;
    cache[4] = (seq_t)1470;
    cache_nmax = 5;
}

/* Ensure that the cache holds terms up to n. */
static void cache_ensure(idx_t n)
{
    if (n <= cache_nmax)
        return;
    idx_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < n)
        new_cap <<= 1;                  /* exponential growth for amortised O(n) */
    cache_expand(new_cap);
    for (idx_t i = cache_nmax + 1; i <= n; ++i) {
        seq_t val = 0;
        val += 7 * cache[i - 2];    /* 7*a(i-1) */
        val -= 9 * cache[i - 3];    /* -9*a(i-2) */
        val -= 1 * cache[i - 4];    /* -a(i-3) */
        val += 9 * cache[i - 5];    /* +9*a(i-4) */
        val -= 3 * cache[i - 6];    /* -3*a(i-5) */
        cache[i - 1] = val;         /* store a(i) at index i-1 */
    }
    cache_nmax = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;              /* undefined for n<1; return 0 safely */
    cache_init();
    cache_ensure(n);
    return cache[n - 1];
}
