/*
 * A381957  If n = Sum 2^e(k), then a(n) = Sum 2^a(e(k)), with a(0) = 1.
 * https://oeis.org/A381957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k where 2^k | n} 2^a(k); a(0) = 1
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation memoizes previously computed values in a static array. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp); */

static seq_t *cache = NULL;      /* cache[i] = a(i)  (0 means not yet computed) */
static size_t cache_len = 0;     /* current allocated size */

/* Ensure that cache has space for indices 0..max_index inclusive. */
static int ensure_cache(size_t max_index)
{
    size_t needed = max_index + 1;
    if (cache_len >= needed)
        return 1;               /* already enough */

    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(*cache));
    if (!tmp)
        return 0;               /* allocation failed */

    /* Zero‑initialize newly added slots. */
    for (size_t i = cache_len; i < needed; ++i)
        tmp[i] = 0;

    cache      = tmp;
    cache_len  = needed;
    return 1;
}

/* Recursive helper that assumes cache is large enough for index n. */
static seq_t a_rec(idx_t n)
{
    if (n == 0) {          /* base case */
        cache[0] = 1;
        return 1;
    }

    /* Return cached value if available. */
    if (cache[n] != 0)
        return cache[n];

    /* Compute a(n) from its binary digits. */
    seq_t sum = 0;
    idx_t pos = 0;
    idx_t tmp = n;

    while (tmp) {
        if (tmp & 1) {
            seq_t exp = cache[pos];
            if (exp == 0) {                 /* ensure a(pos) is known */
                exp = a_rec(pos);           /* recursion will store it */
            }
            /* exp is a(idx_t) because a(pos) never exceeds 64 bits
               for all indices needed in this test harness.          */
            sum += pow2((idx_t)exp);
        }
        tmp >>= 1;
        ++pos;
    }

    cache[n] = sum;    /* memoize */
    return sum;
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (!ensure_cache((size_t)n))
        return 0;      /* allocation failure propagate as 0 */

    return a_rec(n);
}
