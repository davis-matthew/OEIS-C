/*
 * A283607  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 670", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A283607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) for n > 17 * Initial values:
 *            * a(0)=1, a(1)=3, a(2)=7, a(3)=15, a(4)=23, * a(5)=63, a(6)=119,
 *            a(7)=255, a(8)=375, a(9)=1023, * a(10)=1783, a(11)=4095,
 *            a(12)=6647, a(13)=16383, * a(14)=28663, a(15)=65535,
 *            a(16)=131063, a(17)=262143
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Seed values for n = 0..17 */
    static const seq_t init[18] = {
        1ULL, 3ULL, 7ULL, 15ULL, 23ULL,
        63ULL, 119ULL, 255ULL, 375ULL, 1023ULL,
        1783ULL, 4095ULL, 6647ULL, 16383ULL,
        28663ULL, 65535ULL, 131063ULL, 262143ULL
    };

    /* Static cache to remember computed terms */
    static seq_t *cache    = NULL;
    static size_t  cap     = 0;
    static idx_t   size    = 0;   /* number of terms stored (cache_size) */

    /* If n falls in the seed range, return directly */
    if (n < 18) {
        return init[n];
    }

    /* Compute the required size for the cache */
    size_t req = (size_t)n + 1;

    /* Allocate or grow the cache if necessary */
    if (cache == NULL) {
        cap = req;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* memory exhaustion; unlikely */
        for (size_t i = 0; i < 18; ++i) {
            cache[i] = init[i];
        }
        size = 18;
    } else if (req > cap) {
        size_t new_cap = req;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* memory exhaustion */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = size; i <= n; ++i) {
        cache[i] = 2 * cache[i - 1] + cache[i - 2] - 2 * cache[i - 3];
    }
    size = n + 1;

    return cache[n];
}
