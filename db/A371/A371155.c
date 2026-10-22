/*
 * A371155  a(n) depends on the primality of a(n-1) and parity of n (see
 *          Comments lines for definition).
 * https://oeis.org/A371155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 355, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A371155:
   a(1) = 1
   For n > 1:
     if n even and a(n-1) is prime                → a(n) = least prime > a(n-1)
     else if n even, a(n-1) composite and a(n-2) is prime
                             → a(n) = 1 + greatest prime < a(n-2)
     else                                          → a(n) = a(n-1) + 1
   (All arithmetic is on seq_t.) */

#if defined(__cplusplus)
# include <cstdint>
# include <cstdlib>
# include <cstring>
# include <new>
#else
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
#endif

#include "../utils/is_prime_big.h"

/* Least prime strictly greater than x. */
static seq_t
next_prime_gt(seq_t x)
{
    seq_t y = x + 1;
    while (!is_prime_big(y))
        ++y;
    return y;
}

/* Greatest prime strictly less than x.  If x <= 2, returns 0. */
static seq_t
prev_prime_lt(seq_t x)
{
    if (x <= 2u)
        return 0;
    seq_t y = x - 1;
    while (!is_prime_big(y))
    {
        /* y is unsigned; the loop terminates because x > 2. */
        --y;
    }
    return y;
}

/* Static cache of computed terms. Index 0 is unused but kept for
   simplification (a(1) = 1 when cache[1] = 1). */
static seq_t *cache    = NULL;   /* sequence values, 0‑based */
static idx_t  cap      = 0;      /* allocated capacity (size of cache)   */
static idx_t  max_comp = 0;      /* largest index for which a(i) was computed */

/* Ensure cache is large enough to hold indices up to n.  Newly
   allocated slots are zero‑initialized. */
static void
ensure_cache(idx_t n)
{
    if (n + 1 <= cap)
        return;
    /* Allocate a new array of size n+1. */
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();          /* allocation failed */
    /* Zero‑initialize the newly added part. */
    for (idx_t i = cap; i < new_cap; ++i)
        new_cache[i] = 0;
    cache = new_cache;
    cap   = new_cap;
}

/* Return a(n) for any n >= 1.  Subsequent calls are memoized. */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;                        /* undefined for n < 1 */

    /* Extend cache and compute values up to n. */
    ensure_cache(n);
    if (max_comp < n)
    {
        for (idx_t i = max_comp + 1; i <= n; ++i)
        {
            seq_t prev_val    = cache[i - 1];
            seq_t prevprev_val = (i >= 2) ? cache[i - 2] : 0;

            if ((i & 1u) == 0u)         /* i even */
            {
                if (is_prime_big(prev_val))
                {
                    cache[i] = next_prime_gt(prev_val);
                }
                else if (i >= 3u && is_prime_big(prevprev_val))
                {
                    cache[i] = prev_prime_lt(prevprev_val) + 1;
                }
                else
                {
                    cache[i] = prev_val + 1;
                }
            }
            else                         /* i odd */
            {
                cache[i] = prev_val + 1;
            }
        }
        max_comp = n;
    }

    return cache[n];
}
