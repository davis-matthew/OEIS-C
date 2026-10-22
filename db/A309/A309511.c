/*
 * A309511  Number of odd parts in the partitions of n into 3 parts.
 * https://oeis.org/A309511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5) + a(n-6) - a(n-7) - a(n-10) +
 *            a(n-11) * Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=3,
 *            a(4)=2, a(5)=4, a(6)=4, * a(7)=8, a(8)=8, a(9)=13, a(10)=12
 * Range:     u8 n <= 44, u16 n <= 724, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[11] = {0,0,0,3,2,4,4,8,8,13,12};

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Map idx_t to size_t for array indexing; safe for n <= SIZE_MAX */
    size_t idx = (size_t)n;

    /* Ensure cache is allocated and contains values up to idx */
    if (cache_len == 0) {
        /* Allocate at least 11 entries for initial values */
        cache_cap = 32;
        if (cache_cap <= idx) cache_cap = idx + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure fallback */
        for (size_t i = 0; i <= 10; ++i) cache[i] = init_vals[i];
        cache_len = 11;
    } else if (cache_len <= idx) {
        /* Grow cache if needed */
        if (cache_len <= 10) cache_len = 11;
        if (idx + 1 > cache_cap) {
            size_t new_cap = cache_cap * 2;
            while (new_cap <= idx) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* allocation failure fallback */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Extend cache up to idx */
        for (size_t i = cache_len; i <= idx; ++i) {
            if (i <= 10) {
                cache[i] = init_vals[i];
            } else {
                /* Recurrence: a[i] = (a[i-1]+a[i-4]+a[i-6]+a[i-11]) -
                                   (a[i-5]+a[i-7]+a[i-10]) */
                seq_t pos = cache[i-1] + cache[i-4] + cache[i-6] + cache[i-11];
                seq_t neg = cache[i-5] + cache[i-7] + cache[i-10];
                cache[i] = pos - neg;
            }
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
