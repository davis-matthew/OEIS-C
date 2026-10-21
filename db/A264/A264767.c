/*
 * A264767  a(1)=1; for n>1, a(n) = a(n-1) + n if x=0, otherwise a(n) = a(n-1)
 *          / 2^x, where x is the exponent of highest power of 2 dividing
 *          gcd(a(n-1),n).
 * https://oeis.org/A264767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A264767:
   a(1) = 1;
   for n > 1:
      let x = v₂(gcd(a(n-1), n))   (exponent of 2 in the gcd)
      if x = 0      → a(n) = a(n-1) + n
      else          → a(n) = a(n-1) / 2^x   (i.e. shift right by x)
   The function caches previously computed terms so that calls in any
   order are efficient. */

/* mdnf  FIXME: remove the comment after verifying compile */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;     /* cache[i] holds a(i+1) */
static idx_t cache_len = 0;     /* number of elements currently stored */
static idx_t cache_cap = 0;     /* allocated capacity of the buffer */

static void ensure_cache(idx_t upto)
{
    if (upto <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < upto)
        new_cap <<= 1;

    seq_t *new_alloc = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
    if (!new_alloc)
        abort();                     /* cannot recover from OOM */

    if (cache)
    {
        /* copy existing values to new buffer */
        memcpy(new_alloc, cache, (size_t)cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = new_alloc;
    cache_cap = new_cap;
}

/* Return the nth term (1‑based indexing).  The function is safe for any
   idx_t n >= 1 and may be called in arbitrary order. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined, return 0 to avoid crash */
        return 0;

    /* ensure at least one term is cached */
    if (cache_len == 0)
    {
        ensure_cache(1);
        cache[0] = 1;           /* a(1) = 1 */
        cache_len = 1;
    }

    if (n <= cache_len)        /* already computed */
        return cache[n - 1];

    /* compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        seq_t prev = cache[i - 2];   /* a(i-1) */
        seq_t g = gcd(prev, (seq_t)i);
        /* compute exponent of 2 in g */
        unsigned int x = 0;
        /* g is never zero at this point */
        while ((g & 1) == 0)
        {
            g >>= 1;
            ++x;
        }

        seq_t next;
        if (x == 0)
            next = prev + (seq_t)i;
        else
            next = prev >> x;       /* division by 2^x */

        ensure_cache(i);
        cache[i - 1] = next;
        cache_len = i;
    }

    return cache[n - 1];
}
