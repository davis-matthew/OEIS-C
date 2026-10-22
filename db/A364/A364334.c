/*
 * A364334  a(2) = 0; a(n) = a(n-1) + 1 if n is an odd prime; otherwise a(n) =
 *          max{a(k) : k is divisor of n, 1 < k < n}.
 * https://oeis.org/A364334
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 112, u32 n <= 112, u64 n <= 112,
 *            u128 n <= 112, big any n
 * Helpers:   int_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(2) = 0;
   a(n) = a(n-1) + 1 if n is an odd prime;
   otherwise a(n) = max{ a(k) : k|n, 1 < k < n }   */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/int_sqrt.h"

/* Memoisation cache: cache[n] holds a(n) for n ≥ 2.
   cache_len is the number of slots currently allocated. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static idx_t last_computed = 1;   /* highest n that has been computed */

/* Ensure the cache can hold indices 0 … n.  New slots are zero‑initialised. */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_len)
        return;

    size_t new_len = (cache_len == 0 ? 8 : cache_len * 2);
    if (new_len < needed)
        new_len = needed;

    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        abort();                 /* allocation failure – interrupt program */

    /* Zero‑initialize the newly added slots */
    for (size_t i = cache_len; i < new_len; ++i)
        tmp[i] = (seq_t)0;

    cache = tmp;
    cache_len = new_len;
}

/* a(n) – compute the value according to the OEIS definition.
   Any order of calls is allowed; intermediate results are cached. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;  /* value below the offset is undefined */

    ensure_cache(n);

    /* Compute all values up to n that have not yet been cached. */
    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        if (i == 2)          /* initial value */
        {
            cache[i] = (seq_t)0;
            continue;
        }

        if ((i & 1U) && is_prime(i))   /* odd prime */
        {
            cache[i] = cache[i - 1] + (seq_t)1;
        }
        else   /* composite or even > 2 */
        {
            seq_t mx = (seq_t)0;
            idx_t r = int_sqrt(i);
            for (idx_t d = 2; d <= r; ++d)
            {
                if (i % d == 0)
                {
                    if (cache[d] > mx)
                        mx = cache[d];
                    idx_t e = i / d;
                    if (cache[e] > mx)
                        mx = cache[e];
                }
            }
            cache[i] = mx;
        }
    }

    last_computed = n;
    return cache[n];
}
