/*
 * A385617  G.f. A(x) satisfies A(x) = 1/( 1 - x*(A(x) + A(2*x)) ).
 * https://oeis.org/A385617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = Sum_{k=0..n-1} (2^k + 1) * a(k) *
 *            a(n-1-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches values so that calls
   can be made in any order.  It uses only arithmetic
   operations allowed on seq_t and standard memory
   functions.  No file I/O or printing is performed. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms: a[0..] */
static size_t cache_cap = 0;    /* allocated capacity of cache */
static idx_t cached_up_to = (idx_t)-1;  /* highest index for which cache[i] is valid */

/* Ensure the cache can hold values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* first allocation */
        cache_cap = (size_t)64;          /* start with some capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        /* initialize base case */
        cache[0] = (seq_t)1;
        cached_up_to = 0;
    }
    if (n <= cached_up_to) return;      /* already computed */

    /* grow until we can hold n */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;
        cache_cap = new_cap;
    }
    /* compute missing entries */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            /* ck = 2^k + 1 */
            seq_t ck = ((seq_t)1 << k) + (seq_t)1;
            seq_t prod = cache[k];
            prod = prod * cache[i - 1 - k];
            prod = prod * ck;
            sum += prod;
        }
        cache[i] = sum;
    }
    cached_up_to = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
