/*
 * A338749  a(0) = 1; for n > 0, a(n) = n * Sum_{d|n, d < n} a(d - 1) / d.
 * https://oeis.org/A338749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = sum_{d|n, d < n} a(d-1) * (n / d)
 * Range:     u8 n <= 41, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The first 24 terms (for reference) are: a(0)=1, a(1)=0, a(2)=2, a(3)=3, a(4)=4,
   a(5)=5, a(6)=10, a(7)=7, a(8)=14, a(9)=15, a(10)=18, a(11)=11,
   a(12)=39, a(13)=13, a(14)=34, a(15)=37, a(16)=42, a(17)=17,
   a(18)=73, a(19)=19, a(20)=81, a(21)=65, a(22)=58, a(23)=23. */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t computed_max = 0;

seq_t a(idx_t n) {
    if (n == 0) {
        /* Ensure space for a[0] */
        ensure_cache((idx_t)1, &cache, &cache_cap);
        if (computed_max == 0) {
            cache[0] = (seq_t)1;
            computed_max = 0;
        }
        return cache[0];
    }

    /* Ensure capacity up to n */
    ensure_cache(n + 1, &cache, &cache_cap);

    /* Compute any missing values up to n (in increasing order) */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t j = 1; j * j <= i; ++j) {
            if (i % j != 0)
                continue;
            idx_t d1 = j;
            idx_t d2 = i / j;
            if (d1 < i) {
                seq_t term = cache[d1 - 1] * (seq_t)(i / d1);
                sum += term;
            }
            if (d2 != d1 && d2 < i) {
                seq_t term = cache[d2 - 1] * (seq_t)(i / d2);
                sum += term;
            }
        }
        cache[i] = sum;
    }
    computed_max = n;
    return cache[n];
}
