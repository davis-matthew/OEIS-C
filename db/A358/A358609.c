/*
 * A358609  a(n) = Sum_{k=0..floor(n/4)} (-1)^k * (n-4*k)!.
 * https://oeis.org/A358609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A358609
   a(n) = Sum_{k=0..floor(n/4)} (-1)^k * (n-4*k)!
   Recurrence: a(n) = n*a(n-1) - a(n-4) + n*a(n-5),  n > 4
   Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=23
*/

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of computed terms          */
static size_t cache_cap = 0;         /* current allocated size (bytes)   */
static idx_t cache_max = (idx_t)-1;  /* largest index stored (initialized after first call) */

/* Ensure the cache can store values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = ((size_t)n + 1) * 2;   /* double for future growth */
        if (new_cap == 0) new_cap = 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Initialize cache once with the first five terms. */
    if (cache == NULL) {
        /* allocate at least 5 slots (needed for a(4)) */
        ensure_cache(4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)23;
        cache_max = 4;
    }

    /* Make sure capacity covers requested index n. */
    ensure_cache(n);

    /* If already computed, return it. */
    if (n <= cache_max) return cache[n];

    /* Compute terms up to n using the recurrence. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t term = (seq_t)i * cache[i - 1];   /* n * a(n-1) */
        term -= cache[i - 4];                  /* - a(n-4)   */
        term += (seq_t)i * cache[i - 5];       /* + n * a(n-5) */
        cache[i] = term;
    }
    cache_max = n;
    return cache[n];
}
