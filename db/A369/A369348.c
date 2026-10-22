/*
 * A369348  a(1) = 1; for n > 1, a(n) is the smallest positive number such
 *          that a(n) - a(n-1) = sopfr(a(n)) + sopfr(a(n-1)), where sopfr(k)
 *          is the sum of the primes dividing k, with repetition.
 * https://oeis.org/A369348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(1) = 1; for n > 1, a(n) is the smallest positive integer such
 *            that * a(n) - a(n-1) = sopfr(a(n)) + sopfr(a(n-1)), * where
 *            sopfr(k) sums the prime factors of k with multiplicity. * This
 *            function computes the sequence on demand, memoizing results
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the sum of prime factors of x (with multiplicity). */
static seq_t sopfr(seq_t x)
{
    if (x <= (seq_t)1)
        return (seq_t)0;

    seq_t sum = (seq_t)0;
    idx_t p = (idx_t)2;
    /* Trial division up to sqrt(x). */
    for (; (seq_t)p * (seq_t)p <= x; ++p)
    {
        while (x % p == (seq_t)0)
        {
            sum += (seq_t)p;
            x /= (seq_t)p;
        }
    }
    if (x > (seq_t)1)
        sum += x;
    return sum;
}

/* Static cache of computed terms. */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static size_t defined_terms = 0;   /* number of valid terms present in cache */

/* Grow the cache to a new capacity. */
static void grow_cache(size_t new_cap)
{
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();   /* allocation failed */
    cache = tmp;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n = 0 */

    /* Ensure cache is large enough. */
    if (!cache)
    {
        cache_capacity = 16;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache)
            abort();
    }

    /* Extend cache as needed, computing missing terms. */
    while (defined_terms < (size_t)n)
    {
        /* Compute term number defined_terms + 1 (1‑based). */
        if (defined_terms == 0)
        {
            /* a(1) = 1 by definition. */
            cache[0] = (seq_t)1;
            ++defined_terms;
            continue;
        }

        seq_t prev = cache[defined_terms - 1];
        seq_t sp_prev = sopfr(prev);

        seq_t candidate = prev + (seq_t)1;
        seq_t found = (seq_t)0;  /* will hold the term when found */

        /* Search for the smallest candidate satisfying the condition. */
        for (;;)
        {
            /* Detect overflow: if candidate went back to 0, we stop. */
            if (candidate < prev + (seq_t)1)
                break;   /* overflowed */

            seq_t sp_cand = sopfr(candidate);
            seq_t diff = candidate - prev;
            if (diff == sp_prev + sp_cand)
            {
                found = candidate;
                break;
            }
            ++candidate;
        }

        if (found == (seq_t)0)
        {
            /* No further term exists; stop building the sequence. */
            break;
        }

        /* Store the found term. */
        if (defined_terms >= cache_capacity)
            grow_cache(cache_capacity * 2);

        cache[defined_terms] = found;
        ++defined_terms;
    }

    /* Return the requested term if it exists, otherwise 0. */
    if ((size_t)n <= defined_terms)
        return cache[n - 1];
    else
        return (seq_t)0;   /* undefined beyond the known range */
}
