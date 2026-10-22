/*
 * A310096  Coordination sequence Gal.4.15.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5) + a(n-6) -
 *            a(n-7) + a(n-8) - a(n-9) + a(n-10) - a(n-11) Initial values:
 *            a(0)=1, a(1)=3, a(2)=6, a(3)=11, a(4)=15, a(5)=17, a(6)=20,
 *            a(7)=23, a(8)=26, a(9)=33, a(10)=37, a(11)=37
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cap = 0;
static idx_t max_index = 0;

static const seq_t init_vals[12] = { (seq_t)1, (seq_t)3, (seq_t)6, (seq_t)11, (seq_t)15,
                                     (seq_t)17, (seq_t)20, (seq_t)23, (seq_t)26, (seq_t)33,
                                     (seq_t)37, (seq_t)37 };

seq_t a(idx_t n)
{
    /* Lazily initialize the cache with the first 12 terms */
    if (cache == NULL) {
        cap = 12;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed: undefined */
        for (size_t i = 0; i < 12; ++i) cache[i] = init_vals[i];
        max_index = 11;
    }

    if (n <= max_index) return cache[n];

    /* Ensure the cache is large enough */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t new_cap = cap;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failed: undefined */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute new entries up to n */
    for (idx_t i = max_index + 1; i <= n; ++i) {
        seq_t posSum =
            cache[i-1] + cache[i-3] + cache[i-5] + cache[i-6] + cache[i-8] + cache[i-10];
        seq_t res = posSum;
        res -= cache[i-2];
        res -= cache[i-4];
        res -= cache[i-7];
        res -= cache[i-9];
        res -= cache[i-11];
        cache[i] = res;
    }
    max_index = n;
    return cache[n];
}
