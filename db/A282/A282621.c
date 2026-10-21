/*
 * A282621  Number of Eulerian cycles in the graph C_3 X C_n.
 * https://oeis.org/A282621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence a(1)=8, a(2)=320, a(3)=8616, a(4)=207496,
 *            a(5)=4788808
 *            a(n)=47*a(n-1)-742*a(n-2)+4796*a(n-3)-13144*a(n-4)+12320*a(n-5)
 *            for n>5
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* array of computed terms */
static size_t cache_cap = 0;       /* allocated capacity in elements */
static size_t cache_size = 0;      /* number of terms actually computed */

static inline void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 5;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* undefined for 0, return 0 */

    size_t idx = (size_t)n;

    /* handle the first five terms directly */
    if (idx <= 5) {
        static const seq_t init[5] = {8, 320, 8616, 207496, 4788808};
        return init[idx - 1];
    }

    /* initialize cache with the first five values if not yet done */
    if (cache_size == 0) {
        ensure_capacity(5);
        cache[0] = 8;
        cache[1] = 320;
        cache[2] = 8616;
        cache[3] = 207496;
        cache[4] = 4788808;
        cache_size = 5;
    }

    /* extend cache up to the requested index */
    if (idx > cache_size) {
        ensure_capacity(idx);
        for (size_t i = cache_size; i < idx; ++i) {
            cache[i] = 47 * cache[i - 1] - 742 * cache[i - 2]
                     + 4796 * cache[i - 3] - 13144 * cache[i - 4]
                     + 12320 * cache[i - 5];
        }
        cache_size = idx;
    }

    return cache[idx - 1];
}
