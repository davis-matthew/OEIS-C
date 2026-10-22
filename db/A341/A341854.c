/*
 * A341854  Number of triangulations of a fixed hexagon with n internal nodes.
 * https://oeis.org/A341854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A341854: Number of triangulations of a fixed hexagon with n internal nodes.
   Formula: a(n) = 1008 * binomial(4*n+7, n) / ((3*n+8)*(3*n+9))
   Recurrence used for efficient integer computation:
   a(0) = 14, a(1) = 84
   a(n) = (8*(4*n+5)*(2*n+3)*(4*n+7)*(n+1) * a(n-1))
          / (3*n*(3*n+7)*(n+3)*(3*n+8))   for n ≥ 1
   This recurrence yields integer values and keeps intermediate
   sizes manageable for uint64_t results.  Arbitrary‑precision
   arithmetic is handled via the seq_t type and helper mul_small. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_alloc = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache is initialised with a(0) and a(1). */
    if (cache_len == 0)
    {
        cache_alloc = 2;
        cache = (seq_t *)malloc(cache_alloc * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory, unlikely in tests */
        cache[0] = (seq_t)14;
        cache[1] = (seq_t)84;
        cache_len = 2;
    }

    /* Return value if already computed. */
    if (n < cache_len)
        return cache[n];

    /* Grow cache if necessary. */
    if (n >= cache_alloc)
    {
        idx_t new_cap = cache_alloc;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory */
        cache = tmp;
        cache_alloc = new_cap;
    }

    /* Compute subsequent terms. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t result;
        if (sizeof(seq_t) <= 8)   /* 64‑bit or smaller, use 128‑bit intermediate */
        {
            __int128 num = 8;
            num *= (4*i + 5);
            num *= (2*i + 3);
            num *= (4*i + 7);
            num *= (i + 1);
            num *= ( __int128 )cache[i-1];

            __int128 den = 3;
            den *= (i);
            den *= (3*i + 7);
            den *= (i + 3);
            den *= (3*i + 8);

            result = (seq_t)(num / den);
        }
        else   /* arbitrary‑precision seq_t: use mul_small and normal arithmetic */
        {
            seq_t num = cache[i-1];
            num = mul_small(num, 8);
            num = mul_small(num, (4*i + 5));
            num = mul_small(num, (2*i + 3));
            num = mul_small(num, (4*i + 7));
            num = mul_small(num, (i + 1));

            seq_t den = 3;
            den = mul_small(den, i);
            den = mul_small(den, (3*i + 7));
            den = mul_small(den, (i + 3));
            den = mul_small(den, (3*i + 8));

            result = num / den;           /* exact division */
        }

        cache[i] = result;
    }

    cache_len = n + 1;
    return cache[n];
}
