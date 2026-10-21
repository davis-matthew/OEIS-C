/*
 * A258132  Octagonal numbers (A000567) that are the sum of fifteen
 *          consecutive octagonal numbers.
 * https://oeis.org/A258132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 238142*a(n-2) - 238142*a(n-3) - a(n-4) + a(n-5)
 *            Initial terms: a(1)=4715040, a(2)=8463840, a(3)=1122749669280,
 *            a(4)=2015496399840, a(5)=267373851637578960 Recurrence derived
 *            from the generating function denominator
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   free_cache, mul_small, seq_alloc_new_array, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_from_str.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/mul_small.h"
#include "../utils/free_cache.h"

seq_t a(idx_t n)
{
    /* static cache for memoised terms */
    static seq_t *cache = NULL;           /* array of computed terms     */
    static size_t cache_cap = 0;          /* current capacity            */
    static size_t cache_len = 0;          /* number of terms stored (0‑based) */

    if (n == 0)
        return 0;                         /* undefined for n < 1           */

    /* initialise cache with the first 5 terms once */
    if (cache == NULL)
    {
        cache_cap = 16;
        cache = seq_alloc_new_array(cache_cap);
        if (cache == NULL)
            return 0;                    /* allocation failure           */
        cache[0] = seq_from_str("4715040");
        cache[1] = seq_from_str("8463840");
        cache[2] = seq_from_str("1122749669280");
        cache[3] = seq_from_str("2015496399840");
        cache[4] = seq_from_str("267373851637578960");
        cache_len = 5;
    }

    /* grow the cache if necessary */
    if ((size_t)n > cache_len)
    {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n)
            new_cap *= 2;

        if (new_cap > cache_cap)
        {
            seq_t *new_cache = seq_alloc_new_array(new_cap);
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free_cache(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* compute further terms up to n */
        for (size_t idx = cache_len; idx < (size_t)n; ++idx)
        {
            seq_t term1 = cache[idx-1];
            seq_t term2 = mul_small(cache[idx-2], 238142u);
            seq_t term3 = mul_small(cache[idx-3], 238142u);
            seq_t term4 = cache[idx-4];
            seq_t term5 = cache[idx-5];

            seq_t a_n = term1 + term2 - term3 - term4 + term5;
            cache[cache_len++] = a_n;
        }
    }

    return cache[n-1];
}
