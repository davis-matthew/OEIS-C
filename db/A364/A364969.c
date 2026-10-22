/*
 * A364969  a(n) = a(a(n-1)) if n is even, a(n) is the number of times a(n-1)
 *          occurs in the sequence so far if n is odd, with a(1) = 1.
 * https://oeis.org/A364969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A364969 – a(n) = a(a(n‑1)) if n is even,
 *  a(n) = number of times a(n‑1) occurs so far if n is odd,
 *  a(1) = 1.
 *  The function a(idx_t n) implements this recurrence and memoises
 *  results for arbitrary call order.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache: 1‑indexed.  cache[0] is unused. */
static seq_t *cache = NULL;      /* array of computed values */
static idx_t  cache_size = 0;    /* allocated size (>= n+1) */

/* Ensure that the cache has room for indices 0 .. needed (index 0 unused). */
static void ensure_cache(idx_t needed)
{
    if (cache_size >= needed + 1)
        return;

    idx_t new_size = needed + 1;          /* include index 0 */
    /* Allocate new memory */
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)new_size);
    if (!new_cache) {
        /* Allocation failed – abort as the sequence cannot be computed */
        abort();
    }

    /* Copy existing contents */
    idx_t i;
    for (i = 1; i < cache_size && i < new_size; ++i)
        new_cache[i] = cache[i];

    /* Initialize new slots to zero (uncomputed) */
    for (; i < new_size; ++i)
        new_cache[i] = 0;

    /* Free old cache if it existed */
    if (cache != NULL)
        free(cache);

    cache      = new_cache;
    cache_size = new_size;
}

/* Return the n‑th term of the sequence (n ≥ 1).  Undefined for n = 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined – return 0 for safety */

    /* Make sure we can address index n */
    ensure_cache(n);

    /* Initialise first term if not already done */
    if (cache[1] == 0)
        cache[1] = 1;    /* a(1) = 1 */

    /* Compute any missing terms up to n */
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i] != 0)
            continue;      /* already known */

        if ((i & 1ULL) == 0) {
            /* Even index: a(i) = a(a(i‑1)) */
            idx_t target = (idx_t)cache[i - 1];
            ensure_cache(target);   /* the referenced index must be available */
            cache[i] = cache[target];
        } else {
            /* Odd index: a(i) = number of times a(i‑1) appears so far */
            seq_t val = cache[i - 1];
            size_t cnt = 0;
            for (idx_t j = 1; j < i; ++j)
                if (cache[j] == val)
                    ++cnt;
            cache[i] = (seq_t)cnt;
        }
    }

    return cache[n];
}
