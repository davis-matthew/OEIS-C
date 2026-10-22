/*
 * A361041  a(n) = 1680*(3*n)!/(n!*(2*n + 4)!).
 * https://oeis.org/A361041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1680*(3*n)!/(n!*(2*n + 4)!) Recurrence: a(0) = 70; 2*(n +
 *            2)*(2*n + 3)*a(n) = 3*(3*n - 1)*(3*n - 2)*a(n-1)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;         /* array of already computed terms   */
static idx_t cache_len = 0;         /* number of elements stored         */
static size_t cache_cap = 0;        /* actual allocated capacity         */

/*  Compute the sequence from scratch when dynamic memory is unavailable.
    Works for any n >= 0. */
static seq_t compute_from_scratch(idx_t n)
{
    seq_t ans = (seq_t)70;                 /* a(0)            */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t num = (seq_t)3 * (seq_t)(3*k - 1) * (seq_t)(3*k - 2);
        seq_t den = (seq_t)2 * (seq_t)(k + 2) * (seq_t)(2*k + 3);

        seq_t g = gcd(ans, den);
        ans /= g;
        den /= g;

        g = gcd(num, den);
        num /= g;
        den /= g;      /* at this point den should be 1              */

        ans *= num;
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Return immediately if value has already been cached. */
    if (n < cache_len)
        return cache[n];

    /* Ensure we have enough memory for n+1 terms. */
    size_t new_len = (size_t)n + 1;
    if (new_len > cache_cap)
    {
        size_t new_cap = new_len * 2;      /* grow a bit for future calls   */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
        {
            /* Allocation failure: compute without caching. */
            return compute_from_scratch(n);
        }
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Compute all missing terms up to n and store them. */
    for (idx_t k = cache_len; k <= n; ++k)
    {
        if (k == 0)
        {
            cache[0] = (seq_t)70;
            continue;
        }

        seq_t a_prev = cache[k - 1];

        seq_t num = (seq_t)3 * (seq_t)(3*k - 1) * (seq_t)(3*k - 2);
        seq_t den = (seq_t)2 * (seq_t)(k + 2) * (seq_t)(2*k + 3);

        seq_t g = gcd(a_prev, den);
        a_prev /= g;
        den      /= g;

        g = gcd(num, den);
        num /= g;
        den /= g;      /* after reduction den should be 1           */

        cache[k] = a_prev * num;
    }

    cache_len = new_len;
    return cache[n];
}
