/*
 * A265676  a(n) is the total number of petals of the Flower of Life at the
 *          n-th iteration.
 * https://oeis.org/A265676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5); initial
 *            values: a(0)=0, a(1)=1, a(2)=7, a(3)=19, a(4)=43, a(5)=67
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = (idx_t)-1;

/* first six terms */
static const seq_t init_vals[6] = {0, 1, 7, 19, 43, 67};

static void _compute_up_to(idx_t n)
{
    if (last_computed != (idx_t)-1 && n <= last_computed)
        return;                           /* already computed */

    /* grow the cache if necessary */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    idx_t start = (last_computed == (idx_t)-1) ? 0 : last_computed + 1;
    for (idx_t i = start; i <= n; ++i) {
        if (i < 6) {
            seq_cache[i] = init_vals[i];
        } else {
            seq_cache[i] =
                ((seq_t)2 * seq_cache[i-1]
                 - seq_cache[i-2]
                 + seq_cache[i-3]
                 - ((seq_t)2 * seq_cache[i-4])
                 + seq_cache[i-5]);
        }
    }
    last_computed = n;
}

seq_t a(idx_t n)
{
    _compute_up_to(n);
    return seq_cache[n];
}
