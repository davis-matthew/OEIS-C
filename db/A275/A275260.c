/*
 * A275260  Number of n X 2 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,1) or (-1,-1) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A275260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 6*a(n-3) + 6*a(n-4) + 4*a(n-5) -
 *            16*a(n-6) + 12*a(n-7) + 8*a(n-8) - 8*a(n-9) for n > 10 Initial
 *            values: a(1)=2, a(2)=9, a(3)=24, a(4)=64, a(5)=172, a(6)=476,
 *            a(7)=1320, a(8)=3672, a(9)=10220, a(10)=28472
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache       = NULL;   /* array of computed values, cache[0] unused */
static size_t cache_size  = 0;      /* highest index that has been computed */
static size_t cache_cap   = 0;      /* allocated capacity (in indices) */

/* Ensure the cache has at least 'need' indices (need ≥ cache_cap). */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t newcap = cache_cap ? (cache_cap << 1) : 16;
    while (newcap < need)
        newcap <<= 1;

    seq_t *newptr;
    if (cache == NULL)
        newptr = seq_alloc(newcap);                 /* allocate new array */
    else
        newptr = seq_realloc(cache, cache_cap, newcap); /* grow existing array */

    if (!newptr)
        abort();

    cache = newptr;
    cache_cap = newcap;
}

/* Sequence a(n).  n ≥ 1.  Behaviour for n < 1 is unspecified. */
seq_t a(idx_t n)
{
    if (n == 0)      /* safety guard – a(0) not defined by OEIS */
        return 0;

    /* We store values starting at index 1; cache[0] is unused. */
    ensure_cache_cap((size_t)n + 1);   /* need indices 0 … n */

    if ((size_t)n > cache_size)
    {
        /* Base values for n = 1 … 10 */
        static const seq_t base[11] = {
            0,  /* unused index 0 */
            2, 9, 24, 64, 172, 476, 1320, 3672, 10220, 28472
        };

        for (size_t i = cache_size + 1; i <= (size_t)n; ++i)
        {
            if (i <= 10)
            {
                cache[i] = base[(size_t)i];
            }
            else
            {
                /* Recurrence for n > 10 */
                seq_t res = 0;
                res += 4 * cache[i - 1];
                res -= 2 * cache[i - 2];
                res -= 6 * cache[i - 3];
                res += 6 * cache[i - 4];
                res += 4 * cache[i - 5];
                res -= 16 * cache[i - 6];
                res += 12 * cache[i - 7];
                res += 8 * cache[i - 8];
                res -= 8 * cache[i - 9];
                cache[i] = res;
            }
        }
        cache_size = (size_t)n;
    }

    return cache[(size_t)n];
}
