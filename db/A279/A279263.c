/*
 * A279263  Number of n X 3 0..1 arrays with no element equal to a strict
 *          majority of its horizontal and vertical neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + 2*a(n-3) - 4*a(n-4) - a(n-6) for
 *            n>9 with initial values a(1)=2, a(2)=10, a(3)=29, a(4)=86,
 *            a(5)=240, a(6)=626, a(7)=1603, a(8)=4030, a(9)=9973
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 51, u128 n <= 106,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t computed = 0;

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;
    size_t needed = n + 1;
    if (needed < 10)            /* we need space for the first 9 terms */
        needed = 10;

    /* initialise cache on first call */
    if (cache == NULL) {
        ensure_capacity(needed, &cache_cap, (void **)&cache, sizeof(seq_t));
        static const seq_t init[10] = {0,  2,  10,   29,    86,   240,
                                       626, 1603, 4030, 9973};
        for (idx_t i = 1; i <= 9 && i < needed; ++i)
            cache[i] = init[i];
        computed = 9;        /* we have all initial values ready */
        if (n <= 9)
            return cache[n];
    }
    /* enlarge cache if necessary */
    else if (needed > cache_cap) {
        ensure_capacity(needed, &cache_cap, (void **)&cache, sizeof(seq_t));
    }

    /* return cached value if already computed */
    if (n <= computed)
        return cache[n];

    /* compute missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a6 = cache[i - 6];
        seq_t term1 = a1 * 4;
        seq_t term2 = a3 * 2;
        seq_t term3 = a2 * 4;
        seq_t term4 = a4 * 4;
        seq_t tmp  = term1 + term2;
        seq_t sub  = term3 + term4 + a6;
        cache[i] = tmp - sub;
    }
    computed = n;
    return cache[n];
}
