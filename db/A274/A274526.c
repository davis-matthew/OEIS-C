/*
 * A274526  a(n) = ((1 + sqrt(11))^n - (1 - sqrt(11))^n)/sqrt(11).
 * https://oeis.org/A274526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=2; a(n)=2*a(n-1)+10*a(n-2) for n>=2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_size = 0;        /* current allocated capacity */
static idx_t next_to_compute = 0;    /* first index not yet computed */

/* Ensure capacity for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (cache_size <= (size_t)n)
    {
        size_t new_size = cache_size ? (cache_size << 1) : 2;
        while (new_size <= (size_t)n)
            new_size <<= 1;

        if (cache == NULL)
            cache = seq_alloc(new_size);
        else
            cache = seq_realloc(cache, cache_size, new_size);

        cache_size = new_size;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Initialise base values on first use. */
    if (next_to_compute == 0)
    {
        ensure_capacity(1);         /* need room for indices 0 and 1 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        next_to_compute = 2;
    }

    /* Return directly if already computed. */
    if (n < next_to_compute)
        return cache[n];

    /* Make sure the cache is large enough. */
    ensure_capacity(n);

    /* Compute missing terms using the recurrence. */
    for (idx_t i = next_to_compute; i <= n; ++i)
    {
        cache[i] = cache[i-1] * (seq_t)2 + cache[i-2] * (seq_t)10;
    }

    next_to_compute = n + 1;
    return cache[n];
}
