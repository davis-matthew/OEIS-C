/*
 * A268691  Number of n-digit binary strings such that for all 2 <= k <= n,
 *          the string formed by the first k digits of the original string is
 *          composed of at least one-quarter 1's and one-quarter 0's.
 * https://oeis.org/A268691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n‑digit binary strings such that for all 2 ≤ k
 *            ≤ n the count of 0's in the first k bits lies between ⌈k/4⌉ and
 *            ⌊3k/4⌋. The values are computed by dynamic programming over the
 *            number of zeros
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   ceil_quarter, floor_three_quarters
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ceil_quarter.h"
#include "../utils/floor_three_quarters.h"

static seq_t *cur = NULL;
static seq_t *next = NULL;
static size_t cur_sz = 0;
static seq_t *cache = NULL;
static size_t cache_sz = 0;

seq_t a(idx_t n)
{
    /* initialisation */
    if (cache_sz == 0) {
        cur_sz = 1;
        cur = (seq_t *)malloc(cur_sz * sizeof(seq_t));
        next = (seq_t *)malloc(cur_sz * sizeof(seq_t));
        if (!cur || !next) return (seq_t)0;
        cur[0] = (seq_t)1;
        next[0] = (seq_t)0;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;
        cache_sz = 1;
    }

    /* enlarge working arrays if needed for up to n */
    if (cur_sz <= (size_t)n) {
        size_t new_sz = (size_t)n + 1;
        size_t old_sz = cur_sz;
        seq_t *tmp = (seq_t *)realloc(cur, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cur = tmp;
        tmp = (seq_t *)realloc(next, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        next = tmp;
        if (new_sz > old_sz) {
            memset(cur + old_sz, 0, (new_sz - old_sz) * sizeof(seq_t));
            memset(next + old_sz, 0, (new_sz - old_sz) * sizeof(seq_t));
        }
        cur_sz = new_sz;
    }

    /* compute missing terms up to n */
    for (idx_t k = cache_sz; k <= n; ++k) {
        /* zeros range for current prefix length k */
        idx_t low, high;
        if (k == 1) {
            low = 0;
            high = 1;
        } else {
            low = ceil_quarter(k);
            high = floor_three_quarters(k);
        }

        /* clear next array for all usable indices */
        memset(next, 0, cur_sz * sizeof(seq_t));

        /* previous range */
        idx_t prev_low, prev_high;
        if (k == 1) {                /* previous length 0 */
            prev_low = 0;
            prev_high = 0;
        } else if (k == 2) {         /* previous length 1, no restriction */
            prev_low = 0;
            prev_high = 1;
        } else {
            prev_low = ceil_quarter(k - 1);
            prev_high = floor_three_quarters(k - 1);
        }

        /* transition */
        for (idx_t z = prev_low; z <= prev_high; ++z) {
            seq_t val = cur[z];
            if (!val) continue;
            /* append 0 */
            idx_t nz = z + 1;
            if (nz >= low && nz <= high) next[nz] += val;
            /* append 1 */
            if (z >= low && z <= high)   next[z] += val;
        }

        /* swap cur and next */
        seq_t *tmp = cur; cur = next; next = tmp;

        /* sum admissible states for this k */
        seq_t sum = (seq_t)0;
        for (idx_t z = low; z <= high; ++z) sum += cur[z];

        /* extend cache if necessary */
        if (cache_sz <= k) {
            seq_t *new_cache = (seq_t *)realloc(cache, (k + 1) * sizeof(seq_t));
            if (!new_cache) return (seq_t)0;
            cache = new_cache;
            cache_sz = k + 1;
        }
        cache[k] = sum;
    }

    return cache[n];
}
