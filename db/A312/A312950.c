/*
 * A312950  Coordination sequence Gal.6.151.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(0)=1, a(1)=4, a(2)=9, a(3)=13, a(4)=18, a(5)=23,
 *   a(6)=28, a(7)=33, a(8)=38,
 *   a(n)=a(n-1)+a(n-2)-2*a(n-4)+a(n-6)+a(n-7)-a(n-8)   for n>8
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to keep values between calls */
    static seq_t *cache      = NULL;   /* array of computed terms */
    static idx_t computed    = 0;      /* number of terms already computed */
    static idx_t capacity    = 0;      /* allocated size of cache */

    /* Initial terms (indices 0..8) are inserted once */
    if (capacity == 0) {
        capacity  = 9;
        computed  = 9;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failed – return 0 as sentinel */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)13;
        cache[4] = (seq_t)18;
        cache[5] = (seq_t)23;
        cache[6] = (seq_t)28;
        cache[7] = (seq_t)33;
        cache[8] = (seq_t)38;
    }

    /* If the requested index is already known, return it */
    if (n < computed)
        return cache[n];

    /* Grow the cache if necessary */
    if (n + 1 > capacity) {
        idx_t new_capacity = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed – return 0 as sentinel */
        cache = tmp;
        capacity = new_capacity;
    }

    /* Compute missing terms from the last known index up to n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t sum_pos = cache[i-1] + cache[i-2] + cache[i-6] + cache[i-7];
        seq_t sum_neg = (cache[i-4] << 1) + cache[i-8];
        cache[i] = sum_pos - sum_neg;   /* guaranteed non‑negative */
    }
    computed = n + 1;

    return cache[n];
}
