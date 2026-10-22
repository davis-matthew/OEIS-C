/*
 * A375309  Number of walks of length n along the edges of a dodecahedron
 *          graph between two vertices at distance two.
 * https://oeis.org/A375309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 10*a(n-2) - 16*a(n-3) - 25*a(n-4) + 30*a(n-5)
 *            a(0) = 0, a(1) = 0, a(2) = 1, a(3) = 1, a(4) = 7, a(5) = 11
 *            (valid for all n ≥ 0; a(n) is returned as seq_t)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t *cache    = NULL;   /* dynamic array for computed terms   */
static idx_t  alloc_len = 0;     /* allocated array size               */
static idx_t  comp_len  = 0;     /* number of terms that have been computed */

/* Extend the cache so that it contains at least terms up to index n.
   Uncomputed entries are filled using the recurrence.               */
static void ensure_cache(idx_t n)
{
    /* No work needed if all required terms are already computed. */
    if ((n + 1) <= comp_len)
        return;

    /* Grow the allocated array if necessary. */
    if ((n + 1) > alloc_len)
    {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        cache = tmp;
        alloc_len = n + 1;
    }

    /* Fill in the missing terms, from the first not-yet-computed up
       to the requested n. */
    for (idx_t i = comp_len; i <= n; ++i)
    {
        switch (i)
        {
            case 0: cache[i] = (seq_t)0; break;
            case 1: cache[i] = (seq_t)0; break;
            case 2: cache[i] = (seq_t)1; break;
            case 3: cache[i] = (seq_t)1; break;
            case 4: cache[i] = (seq_t)7; break;
            case 5: cache[i] = (seq_t)11; break;
            default:
            {
                seq_t t1 = mul_small(cache[i-1], 2U);
                seq_t t2 = mul_small(cache[i-2], 10U);
                seq_t t3 = mul_small(cache[i-5], 30U);
                seq_t t4 = mul_small(cache[i-3], 16U);
                seq_t t5 = mul_small(cache[i-4], 25U);
                cache[i] = (t1 + t2 + t3) - (t4 + t5);
            }
            break;
        }
    }

    comp_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
