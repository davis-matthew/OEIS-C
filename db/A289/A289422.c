/*
 * A289422  Number of Dyck paths of semilength n and height exactly 9.
 * https://oeis.org/A289422
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of Dyck paths of semilength n and height exactly 9
 *            satisfies the linear recurrence a(n) = 17*a(n-1) - 121*a(n-2) +
 *            468*a(n-3) - 1068*a(n-4) + 1461*a(n-5) - 1163*a(n-6) +
 *            496*a(n-7) - 95*a(n-8) + 5*a(n-9) for n ≥ 10, with
 *            a(0)=a(1)=…=a(8)=0 and a(9)=1. The recurrence comes directly
 *            from the...
 * Range:     u8 n <= 11, u16 n <= 14, u32 n <= 21, u64 n <= 37, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t next_to_compute = 10;   /* next index that needs computation */

    /* Values for n < 9 are 0 */
    if (n < 9u)
        return (seq_t)0;

    /* Initial allocation of the cache */
    if (!cache)
    {
        cache_size = 20;                      /* arbitrary initial size */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            abort();                          /* memory allocation failed */
        /* Initialise first ten entries (0..9) */
        for (idx_t i = 0; i < 10u; ++i)
            cache[i] = (seq_t)0;
        cache[9] = (seq_t)1;                  /* a(9) = 1 */
        next_to_compute = 10u;                /* we have values up to 9 */
    }

    /* Grow the cache if necessary */
    if (n >= cache_size)
    {
        idx_t new_cap = cache_size * 2u;
        while (new_cap <= n)
            new_cap *= 2u;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort();                          /* memory allocation failed */
        cache = new_cache;
        cache_size = new_cap;
    }

    /* Compute values up to n if they are not yet in the cache */
    while (next_to_compute <= n)
    {
        const idx_t i = next_to_compute;

        /* Positive contributions (coefficients > 0) */
        seq_t pos = 17u * cache[i - 1u];
        pos += 468u * cache[i - 3u];
        pos += 1461u * cache[i - 5u];
        pos += 496u * cache[i - 7u];
        pos += 5u * cache[i - 9u];

        /* Negative contributions (coefficients < 0) */
        seq_t sub = 121u * cache[i - 2u];
        sub += 1068u * cache[i - 4u];
        sub += 1163u * cache[i - 6u];
        sub += 95u * cache[i - 8u];

        /* Result is the positive part minus the negative part */
        cache[i] = pos - sub;

        ++next_to_compute;
    }

    return cache[n];
}
