/*
 * A269448  The first of 26 consecutive positive integers the sum of the
 *          squares of which is a square.
 * https://oeis.org/A269448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 102*a(n-3) - a(n-6) + 1250 Initial terms: a(1)=25,
 *            a(2)=301, a(3)=454, a(4)=3850, a(5)=31966, a(6)=47569
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[7] = {
    0,
    (seq_t)25,   /* a(1) */
    (seq_t)301,  /* a(2) */
    (seq_t)454,  /* a(3) */
    (seq_t)3850, /* a(4) */
    (seq_t)31966,/* a(5) */
    (seq_t)47569 /* a(6) */
};

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap  = 0;

static void ensure_capacity(idx_t n)
{
    if (cache_cap < n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n <= 6) return init[n];

    ensure_capacity(n);

    if (cache_size < n) {
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 6)
                cache[i] = init[i];
            else
                cache[i] = (seq_t)102 * cache[i - 3] - cache[i - 6] + (seq_t)1250;
        }
        cache_size = n;
    }

    return cache[n];
}
