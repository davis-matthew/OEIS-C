/*
 * A334578  Double subfactorials: a(n) = (-1)^floor(n/2) * n!! *
 *          Sum_{i=0..floor(n/2)} (-1)^i/(n-2*i)!!.
 * https://oeis.org/A334578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 33, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(n)=n*a(n-2)+(-1)^floor(n/2) for n>=2 */
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t last_computed = 1;

/* Ensure the cache is allocated and initialized up to index n. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap) return;
    size_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return;            /* out of memory – caller will fail naturally */
    for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
    for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0; /* uncomputed */
    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialize cache to store at least a(0) and a(1). */
static void init_cache(void)
{
    if (cache) return;
    cache = (seq_t *)malloc(2 * sizeof(seq_t));
    cache[0] = 1;
    cache[1] = 1;
    cache_cap = 2;
    last_computed = 1;
}

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return 1;
    init_cache();
    if ((size_t)n >= cache_cap) ensure_cache(n);
    if (n <= last_computed) return cache[n];

    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        seq_t prev = cache[i - 2];
        seq_t val = ((seq_t)i) * prev;                 /* n * a(n-2) */
        if (((i / 2) % 2) == 0) {                       /* floor(n/2) even -> add 1 */
            val = val + 1;
        } else {                                        /* odd -> subtract 1 */
            val = val - 1;
        }
        cache[i] = val;
    }
    last_computed = n;
    return cache[n];
}
