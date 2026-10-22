/*
 * A397649  Number of ways to tile a 3 X n strip with squares, dominoes, and
 *          crosses.
 * https://oeis.org/A397649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 14*a(n-2) + 4*a(n-3) - 11*a(n-4) - 8*a(n-5) +
 *            a(n-6) + a(n-7); Initial values: a(0)=1, a(1)=3, a(2)=22,
 *            a(3)=132, a(4)=839, a(5)=5252, a(6)=33017
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* sequence cache */
    static idx_t cache_len = 0;   /* number of cached entries */

    /* Ensure cache can hold up to n */
    idx_t required = n + 1;
    if (required > cache_len) {
        /* Allocate new larger buffer */
        seq_t *tmp = alloc_cache(required);
        /* Copy existing values */
        for (idx_t i = 0; i < cache_len; ++i)
            tmp[i] = cache[i];

        /* Fill in new entries */
        for (idx_t i = cache_len; i < required; ++i) {
            if (i < 7) {             /* base cases */
                switch (i) {
                    case 0: tmp[0] = 1;            break;
                    case 1: tmp[1] = 3;            break;
                    case 2: tmp[2] = 22;           break;
                    case 3: tmp[3] = 132;          break;
                    case 4: tmp[4] = 839;          break;
                    case 5: tmp[5] = 5252;         break;
                    case 6: tmp[6] = 33017;        break;
                }
            } else {                     /* recurrence */
                seq_t val = 4 * tmp[i-1];
                val += 14 * tmp[i-2];
                val += 4 * tmp[i-3];
                val += tmp[i-6];
                val += tmp[i-7];
                val -= 11 * tmp[i-4];
                val -= 8 * tmp[i-5];
                tmp[i] = val;
            }
        }

        /* Free old cache and replace */
        if (cache != NULL)
            free_cache(cache);
        cache = tmp;
        cache_len = required;
    }

    return cache[n];
}
