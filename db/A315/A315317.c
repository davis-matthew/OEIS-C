/*
 * A315317  Coordination sequence Gal.5.257.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A315317 – Coordination sequence Gal.5.257.3
   Formula (recurrence, offset 0):
   a(0) = 1, a(1) = 6, a(2) = 10, a(3) = 16, a(4) = 21, a(5) = 25,
   a(6) = 32, a(7) = 37, a(8) = 41
   a(n) = a(n-1) - a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5)
          - a(n-6) + a(n-7) - a(n-8)   for n > 8
*/

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoization buffer          */
static size_t cache_cap = 0;         /* allocated capacity          */
static size_t cache_len = 0;         /* number of already computed  */

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;

#if defined(__cplusplus)
    seq_t *new_cache = new seq_t[new_cap];
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    delete[] cache;
    cache = new_cache;
#else
    seq_t *new_cache = realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
#endif
    cache_cap = new_cap;
}

/* Coordination sequence Gal.5.257.3  (A315317) */
seq_t a(idx_t n)
{
    static const seq_t base[9] = {1, 6, 10, 16, 21, 25, 32, 37, 41};

    size_t target = (size_t)n;
    if (target >= cache_cap)
        ensure_capacity(target + 1);

    for (size_t i = cache_len; i <= target; ++i) {
        if (i < 9) {
            cache[i] = base[i];
        } else {
            /* compute using the recurrence in a safe unsigned way */
            seq_t sum1 = cache[i - 1];
            sum1 += cache[i - 3] + cache[i - 3];
            sum1 += cache[i - 5] + cache[i - 5];
            sum1 += cache[i - 7];

            seq_t sum2 = cache[i - 2];
            sum2 += cache[i - 4] + cache[i - 4];
            sum2 += cache[i - 6];
            sum2 += cache[i - 8];

            cache[i] = sum1 - sum2;   /* sum1 >= sum2 by definition */
        }
    }
    cache_len = target + 1;
    return cache[(size_t)n];
}
