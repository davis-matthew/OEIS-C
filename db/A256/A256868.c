/*
 * A256868  Numbers divisible by prime(d+1) for each digit d of their base-8
 *          representation.
 * https://oeis.org/A256868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth smallest non‑negative integer x such that for each
 *            octal digit d of x, x is divisible by prime(d+1). The primes are
 *            (2,3,5,7,11,13,17,19) for d = 0..7. The sequence is built by a
 *            linear scan with caching
 * Range:     u8 n <= 7, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* The prime corresponding to each octal digit d = 0..7. */
static const unsigned int _primes[8] = {2,3,5,7,11,13,17,19};

/* Cache of found sequence terms. */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* allocated capacity */

/* The smallest candidate not yet examined. */
static idx_t examined = 0;

/* Grow the cache array to hold at least 'needed' elements. */
static void _grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)
    {
        /* Allocation failed – fall back to zero capacity. */
        cache = NULL;
        cache_cap = cache_size = 0;
        return;
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Test whether a candidate value satisfies the divisibility condition. */
static bool _accepts(uint64_t val)
{
    uint64_t v = val;
    unsigned mask = 0;

    /* Build a bitmask of the octal digits that appear in v. */
    while (v != 0)
    {
        unsigned digit = (unsigned)(v % 8);   /* digit ∈ [0,7] */
        mask |= 1u << digit;
        v /= 8;
    }

    /* Compute the least common multiple (LCM) of the required primes. */
    uint32_t lcm = 1;
    for (unsigned i = 0; i < 8; ++i)
        if (mask & (1u << i))
            lcm *= _primes[i];

    return ((uint64_t)val % lcm) == 0;
}

/* Return the nth term of the sequence (n >= 1).  For n == 0 the
   function returns 0, but the OEIS offset starts at 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Grow cache up to size n if necessary. */
    if ((idx_t)cache_size < n)
    {
        _grow_cache((size_t)n);

        for (idx_t cand = examined; ; ++cand)
        {
            /* Only store the candidate if it fits into seq_t.  For
               the small-build (uint8_t) this keeps the cache free of
               wrapped values that would corrupt the ordering of
               subsequent terms.  For larger seq_t types this test
               is effectively always true. */
            if (cand <= (uint64_t)(-1))
            {
                if (_accepts((uint64_t)cand))
                    cache[cache_size++] = (seq_t)cand;
            }

            if ((idx_t)cache_size >= n)
            {
                examined = cand + 1;
                break;
            }
        }
    }

    return cache[n - 1];
}
