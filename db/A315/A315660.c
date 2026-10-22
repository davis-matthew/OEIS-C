/*
 * A315660  Coordination sequence Gal.5.320.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-9) - a(n-10) for n > 15 Initial values:
 *            a(0)=1, a(1)=6, a(2)=12, a(3)=16, a(4)=23, a(5)=28, a(6)=35,
 *            a(7)=40, a(8)=46, a(9)=52, a(10)=58, a(11)=64, a(12)=68,
 *            a(13)=76, a(14)=81, a(15)=87
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* constant initial block */
    static const seq_t init[16] = {
        1, 6, 12, 16, 23, 28, 35, 40,
        46, 52, 58, 64, 68, 76, 81, 87
    };

    /* static cache for memoisation */
    static seq_t *cache      = NULL;   /* array of seq_t */
    static size_t  cache_cap = 0;      /* allocated length */
    static idx_t   upto      = 15;     /* highest index already computed */

    /* direct lookup for first 16 values */
    if (n < 16) {
        return init[(idx_t)n];
    }

    /* first access: allocate and initialise base values */
    if (!cache) {
        cache = seq_alloc((size_t)(n + 1));
        if (!cache) exit(EXIT_FAILURE);
        for (size_t i = 0; i < 16; ++i) {
            cache[i] = init[i];
        }
        cache_cap = n + 1;
    }
    /* grow cache if required */
    else if (cache_cap < (size_t)(n + 1)) {
        seq_t *tmp = seq_realloc(cache, cache_cap, (size_t)(n + 1));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = n + 1;
    }

    /* compute on demand */
    if (upto < n) {
        for (idx_t i = upto + 1; i <= n; ++i) {
            cache[i] = cache[i - 1];
            cache[i] += cache[i - 9];
            cache[i] -= cache[i - 10];
        }
        upto = n;
    }

    return cache[n];
}
