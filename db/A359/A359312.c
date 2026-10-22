/*
 * A359312  a(1) = 1; for n >= 1, a(2*n) = A000005(a(n)), a(2*n + 1) =
 *          A000005(a(n)) + 1.
 * https://oeis.org/A359312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1;
   for n≥1, a(2*n)   = A000005(a(n));
   a(2*n+1) = A000005(a(n)) + 1;
   where A000005(m) counts the positive divisors of m. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Compute the number of positive divisors of a seq_t value v. */
static seq_t divisor_count(seq_t v)
{
    if (v <= (seq_t)1)
        return (seq_t)1;

    seq_t rem  = v;
    seq_t cnt  = (seq_t)1;
    seq_t sr   = floor_sqrt(rem);

    for (seq_t i = (seq_t)2; i <= sr; ++i)
    {
        if (rem % i == (seq_t)0)
        {
            unsigned int exp = 0;
            while (rem % i == (seq_t)0)
            {
                rem /= i;
                ++exp;
            }
            cnt *= (seq_t)(exp + 1);
        }
    }
    if (rem > (seq_t)1)          /* last prime factor */
        cnt *= (seq_t)2;
    return cnt;
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (n == (idx_t)0)           /* undefined, but guard */
        return (seq_t)0;

    if ((size_t)n > cache_size)
    {
        size_t new_size = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            abort();             /* fatal if memory allocation fails */
        for (size_t i = cache_size; i < new_size; ++i)
            new_cache[i] = (seq_t)0;  /* unused sentinel */
        cache = new_cache;
        cache_size = new_size;
    }

    seq_t *slot = cache + (size_t)(n - (idx_t)1);
    if (*slot != (seq_t)0)
        return *slot;            /* cached value */

    seq_t result;
    if (n == (idx_t)1)
    {
        result = (seq_t)1;
    }
    else if ((n & (idx_t)1) == (idx_t)0)   /* even n = 2*k */
    {
        result = divisor_count(a(n >> 1));
    }
    else                                   /* odd n = 2*k + 1 */
    {
        result = divisor_count(a(n >> 1)) + (seq_t)1;
    }

    *slot = result;
    return result;
}
