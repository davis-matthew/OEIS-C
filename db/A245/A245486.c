/*
 * A245486  Product of the greatest prime factor of n and the greatest prime
 *          factor of n+1.
 * https://oeis.org/A245486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = gpf(n) * gpf(n+1)
 * Range:     u8 n <= 36, u16 n <= 381, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache of greatest prime factors up to the current maximum index */
static size_t cache_size = 0;
static uint64_t *gpf_cache = NULL;

/* Builds or extends the cache up to and including max_index */
static void ensure_cache(size_t max_index)
{
    /* if already big enough, nothing to do */
    if (max_index + 1 <= cache_size)
        return;

    /* allocate new cache */
    size_t new_size = max_index + 1;
    uint64_t *new_cache = (uint64_t *)malloc(new_size * sizeof(uint64_t));
    if (!new_cache)
        abort();          /* out of memory – stop the program */

    /* initialise all entries to 0; we will fill them with a sieve */
    for (size_t i = 0; i < new_size; ++i)
        new_cache[i] = 0;

    /* free the old cache, if any */
    free(gpf_cache);
    gpf_cache = new_cache;
    cache_size = new_size;

    /* special case for n = 1 */
    if (cache_size > 1)
        gpf_cache[1] = 1;

    /* sieve to find the largest prime factor of every integer */
    for (size_t p = 2; p < cache_size; ++p)
    {
        if (gpf_cache[p] == 0)          /* p is prime */
        {
            gpf_cache[p] = p;          /* the prime itself */
            for (size_t m = p; m < cache_size; m += p)
                gpf_cache[m] = p;      /* overwrite with the larger prime tail */
        }
    }
}

seq_t a(idx_t n)
{
    /* make sure we have gpf information for n and n+1 */
    ensure_cache((size_t)n + 1);

    seq_t g1 = (seq_t)gpf_cache[(size_t)n];
    seq_t g2 = (seq_t)gpf_cache[(size_t)(n + 1)];
    return g1 * g2;
}
