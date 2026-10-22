/*
 * A349956  Number of ways to write n as x^2 + 2*y^2 + (z^4 + 4*w^4)/5 with
 *          x,y,z,w nonnegative integers.
 * https://oeis.org/A349956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the number of quadruples (x, y, z, w) of
 *            non‑negative integers satisfying n = x^2 + 2*y^2 + (z^4 +
 *            4*w^4)/5
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static inline idx_t pow4(idx_t a) {
    idx_t aa = a * a;
    return aa * aa;
}

static void compute_counts(seq_t *arr, idx_t limit) {
    /* Zero the array (should already be zeroed by calloc, but keep for safety) */
    memset(arr, 0, (limit + 1) * sizeof(seq_t));

    idx_t limit5 = 5 * limit;
    for (idx_t x = 0; ; ++x) {
        idx_t x2 = x * x;
        if (x2 > limit) break;

        for (idx_t w = 0; ; ++w) {
            idx_t w4 = pow4(w);
            idx_t fourw4 = 4 * w4;
            if (fourw4 > limit5) break;

            for (idx_t z = 0; ; ++z) {
                idx_t z4 = pow4(z);
                idx_t maxz4 = limit5 - fourw4;
                if (z4 > maxz4) break;
                if ((z4 + fourw4) % 5 != 0) continue;

                idx_t v = (z4 + fourw4) / 5;
                idx_t base = x2 + v;
                if (base > limit) continue;

                for (idx_t y = 0; ; ++y) {
                    idx_t y2 = y * y;
                    idx_t term = base + 2 * y2;
                    if (term > limit) break;
                    arr[term] += 1;
                }
            }
        }
    }
}

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n) {
    if (n >= cache_size) {
        idx_t newsize = n + 1;
        seq_t *new_cache = (seq_t *)calloc(newsize, sizeof(seq_t));
        if (!new_cache) {
            /* In the unlikely event of an allocation failure, terminate */
            exit(1);
        }
        compute_counts(new_cache, n);
        if (cache) free(cache);
        cache = new_cache;
        cache_size = newsize;
    }
    return cache[n];
}
