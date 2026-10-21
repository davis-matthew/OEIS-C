/*
 * A282990  Number of nX2 0..1 arrays with no 1 equal to more than one of its
 *          horizontal and vertical neighbors.
 * https://oeis.org/A282990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - a(n-4) (empirical recurrence,
 *            a(1)=4, a(2)=11, a(3)=33, a(4)=98)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0] unused; cache[1].. are terms */
static idx_t cached_size = 0; /* largest index stored (0 if none) */

/* Expand the cache so that it contains at least terms up to 'up_to' */
static void ensure_cache(idx_t up_to)
{
    if (cached_size >= up_to)
        return;

    /* Reallocate memory (size is up to+1 because index 0 is unused) */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)(up_to + 1) * sizeof(seq_t));
    if (!tmp) abort();               /* OOM – stop execution */
    cache = tmp;

    /* Fill any newly created entries */
    for (idx_t i = cached_size + 1; i <= up_to; ++i) {
        if (i == 1)
            cache[i] = (seq_t)4;
        else if (i == 2)
            cache[i] = (seq_t)11;
        else if (i == 3)
            cache[i] = (seq_t)33;
        else if (i == 4)
            cache[i] = (seq_t)98;
        else
            cache[i] = 2 * cache[i - 1] + 3 * cache[i - 2] - cache[i - 4];
    }

    cached_size = up_to;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* out‑of‑range */

    ensure_cache(n);
    return cache[n];
}
