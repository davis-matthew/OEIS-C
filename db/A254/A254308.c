/*
 * A254308  a(n) = a(n-1) + (if a(n-1) is odd a(n-2) else a(n-3)) with a(0) =
 *          0, a(1) = 1.
 * https://oeis.org/A254308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 52, u64 n <= 102,
 *            u128 n <= 203, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 1
   a(n) = a(n-1) + ( a(n-1) is odd ? a(n-2) : a(n-3) )   for n ≥ 2   */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;   /* array of computed terms        */
static idx_t cache_cap = 0;   /* allocated size in elements     */
static idx_t cache_len = 0;   /* number of filled elements      */

/* ensure the cache can hold indices 0 … n */
static void ensure_cache_cap(idx_t n)
{
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) { /* out‑of‑memory: abort for safety */
            abort();
        }
        cache = new_ptr;
        cache_cap = new_cap;
    }
}

/* OEIS A254308 */
seq_t a(idx_t n)
{
    /* Base terms */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* Initialise first two terms on first use */
    if (cache_len == 0) {
        /* Need storage for indices 0 and 1 */
        ensure_cache_cap(1);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    if (n < cache_len)           /* already computed */
        return cache[n];

    /* Extend cache if necessary */
    ensure_cache_cap(n);

    /* Compute terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t add = (prev & (seq_t)1) ? cache[i - 2] : cache[i - 3];
        cache[i] = prev + add;
    }
    cache_len = n + 1;

    return cache[n];
}
