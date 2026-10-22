/*
 * A379508  Sum of the legs of the unique primitive Pythagorean triple (x,y,z)
 *          such that (x-y+z)/2 is A002315(n) and such that its long leg and
 *          its hypotenuse are consecutive natural numbers.
 * https://oeis.org/A379508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A002315(n)^2 - 1 (equivalently) Recurrence: a(0)=1,
 *            a(1)=97, a(2)=3361, a(n) = 35*a(n-1) - 35*a(n-2) + a(n-3) for
 *            n≥3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* Static cache to store previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements cached (size) */

    /* Initialise cache with first three terms if necessary */
    if (cache_size == 0)
    {
        cache_size = 3;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)97;
        cache[2] = (seq_t)3361;
    }

    /* If the requested n is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* Extend the cache to cover at least up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* Compute new values using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        /* a(i) = 35*a(i-1) - 35*a(i-2) + a(i-3)  */
        seq_t t1 = cache[i - 1] * (seq_t)35;
        seq_t t2 = cache[i - 2] * (seq_t)35;
        seq_t t3 = cache[i - 3];
        cache[i] = t1 - t2 + t3;
    }

    cache_size = new_size;
    return cache[n];
}
