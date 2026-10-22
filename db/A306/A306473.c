/*
 * A306473  a(n) is the maximum number of distinct palindromic not necessarily
 *          contiguous subwords over all binary words of length n.
 * https://oeis.org/A306473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-4) - a(n-5) for n>9, with
 *            initial terms: a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=7, a(6)=10,
 *            a(7)=14, a(8)=18, a(9)=25. (Conjecture known to hold for all
 *            computed values.)
 * Range:     u8 n <= 18, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[10] = {0, 1, 2, 3, 5, 7, 10, 14, 18, 25};

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t max_computed = 0;

seq_t a(idx_t n)
{
    if (n <= 9) {
        return init[n];
    }

    /* Ensure the cache is large enough. */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) {
            abort(); /* memory allocation failed */
        }

        /* Copy previously computed values. */
        if (cache) {
            for (idx_t i = 0; i < cache_cap; ++i) {
                tmp[i] = cache[i];
            }
            free(cache);
        }

        /* Fill missing initial values if necessary. */
        for (idx_t i = cache_cap; i <= 9; ++i) {
            tmp[i] = init[i];
        }

        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* At this point, cache is big enough. */
    if (max_computed < 9) {
        max_computed = 9;
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Compute using the recurrence while avoiding unsigned underflow. */
        seq_t v = cache[i - 1] + cache[i - 2] + cache[i - 4];
        v -= cache[i - 3] + cache[i - 5];
        cache[i] = v;
    }

    max_computed = n;
    return cache[n];
}
