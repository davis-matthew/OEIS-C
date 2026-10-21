/*
 * A242410  a(1)=1 and for n>1, a(n) is the smallest number greater than
 *          a(n-1) such that a(n) is not divisible by a(d) for any divisor d
 *          of n (except 1 and n).
 * https://oeis.org/A242410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A242410: a(1)=1 and for n>1,
   a(n) = the smallest integer greater than a(n-1)
          which is not divisible by a(d) for any divisor d of n
          (excluding 1 and n). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Small cache to memoise previously computed terms.
   cache[0] holds a(1), cache[1] holds a(2), etc. */
static seq_t *cache   = NULL;   /* array of seq_t */
static size_t cache_cap = 0;    /* number of elements allocated */

/* Ensure that the cache is large enough for indices up to n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= (size_t)n)
        return;

    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
    while (new_cap < (size_t)n)
        new_cap <<= 1;                /* grow geometrically */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* If realloc fails, we leak – the caller will not retry. */
    if (tmp == NULL)
        return;

    /* Initialise the newly allocated slots to 0. */
    for (size_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = (seq_t)0;

    cache      = tmp;
    cache_cap  = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* out of bounds – undefined   */

    ensure_cache(n);
    idx_t i = n - 1;                /* zero‑based index in cache   */
    if (cache[i] != (seq_t)0)       /* already computed            */
        return cache[i];

    /* Base case a(1)=1 */
    if (n == 1) {
        cache[0] = (seq_t)1;
        return cache[0];
    }

    /* Compute a(n) using recursion for divisors and preceding term. */
    seq_t prev      = a(n - 1);                 /* a(n-1) */
    seq_t candidate = prev + (seq_t)1;          /* must be > a(n-1) */

    while (true) {
        bool ok = true;
        /* Check all proper divisors d of n (1 and n excluded). */
        for (idx_t d = 2; d <= n / 2; ++d) {
            if (n % d == 0) {
                seq_t ad = a(d);                /* a(d) */
                if (candidate % ad == (seq_t)0) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok) {
            cache[i] = candidate;
            return candidate;
        }
        ++candidate;                           /* try next integer    */
    }
}
