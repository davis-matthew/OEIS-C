/*
 * A313255  Coordination sequence Gal.5.87.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10) for n > 10
 *            (i.e., n ≥ 11). Initial values: a(0)=1, a(1)=4, a(2)=9, a(3)=15,
 *            a(4)=20, a(5)=25, a(6)=30, a(7)=35, a(8)=41, a(9)=46, a(10)=50
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cached sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Seed values for n = 0..10 */
static const seq_t seed[] = {
    (seq_t)1, (seq_t)4, (seq_t)9, (seq_t)15, (seq_t)20,
    (seq_t)25, (seq_t)30, (seq_t)35, (seq_t)41, (seq_t)46, (seq_t)50
};
static const size_t seed_len = sizeof(seed) / sizeof(seed[0]);

seq_t a(idx_t n)
{
    /* Ensure cache has space for indices up to n */
    if (cache_len <= (size_t)n) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            if (cache == NULL) {
                cache = (seq_t *)malloc(needed * sizeof(seq_t));
                if (cache == NULL) exit(EXIT_FAILURE);
            } else {
                seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
                if (tmp == NULL) exit(EXIT_FAILURE);
                cache = tmp;
            }
            cache_cap = needed;
        }

        /* Compute missing terms */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i < seed_len) {
                cache[i] = seed[i];
            } else {
                /* Recurrence for n ≥ 11 */
                size_t k = i;                     /* use a local variable for clarity */
                seq_t pos = 2 * (cache[k-1] + cache[k-3] + cache[k-5]
                                 + cache[k-7] + cache[k-9]);
                seq_t neg = 2 * (cache[k-2] + cache[k-4] + cache[k-6]
                                 + cache[k-8]) + cache[k-10];
                cache[i] = pos - neg;
            }
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
