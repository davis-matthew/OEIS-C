/*
 * A297817  Number of nX2 0..1 arrays with every element equal to 1, 2 or 3
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + 2*a(n-3) - 2*a(n-4) - 4*a(n-5) with
 *            a(1)=1, a(2)=4, a(3)=17, a(4)=49, a(5)=166, a(6)=573
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"
#include <stddef.h>

static seq_t *cache          = NULL;
static size_t cache_cap      = 0;
static size_t computed_up_to = 0;
static const seq_t base_values[7] = {0,
                                    (seq_t)1, (seq_t)4, (seq_t)17,
                                    (seq_t)49, (seq_t)166, (seq_t)573};

seq_t a(idx_t n)
{
    /* Handle offset and trivial cases */
    if (n <= 6)
        return base_values[(size_t)n];

    /* Ensure buffer is large enough */
    size_t need = (size_t)n + 1;
    if (cache_cap < need) {
        seq_t *new_cache = seq_alloc_new_array(need);
        if (cache) {
            for (size_t i = 1; i < cache_cap; ++i)
                new_cache[i] = cache[i];
            seq_free(cache);
        }
        cache     = new_cache;
        cache_cap = need;

        /* Initialize known base terms if they were not yet set */
        if (computed_up_to < 6) {
            for (size_t i = 1; i <= 6; ++i)
                cache[i] = base_values[i];
        }
    }

    /* Compute missing values up to n */
    if (computed_up_to < (size_t)n) {
        for (size_t i = computed_up_to + 1; i <= (size_t)n; ++i) {
            if (i <= 6) {          /* Should already match base_values */
                cache[i] = base_values[i];
            } else {
                seq_t v = 3 * cache[i - 1] + cache[i - 2] + 2 * cache[i - 3];
                v = v - (2 * cache[i - 4]) - (4 * cache[i - 5]);
                cache[i] = v;
            }
        }
        computed_up_to = (size_t)n;
    }

    return cache[(size_t)n];
}
