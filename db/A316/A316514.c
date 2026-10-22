/*
 * A316514  Number of nX4 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +3*a(n-2) +6*a(n-3) +3*a(n-4) -a(n-5) -5*a(n-6)
 *            -4*a(n-7) -4*a(n-8) +2*a(n-10) for n>13, with initial values
 *            a(1)=8, a(2)=49, a(3)=75, a(4)=204, a(5)=619, a(6)=1710,
 *            a(7)=4894, a(8)=14053, a(9)=40063, a(10)=114638, a(11)=327924,
 *            a(12)=937707, a(13)=2681815
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[13] = {
    (seq_t)8, (seq_t)49, (seq_t)75, (seq_t)204, (seq_t)619,
    (seq_t)1710, (seq_t)4894, (seq_t)14053, (seq_t)40063,
    (seq_t)114638, (seq_t)327924, (seq_t)937707, (seq_t)2681815
};

static seq_t *cache = NULL;      /* cache[i] = a(i+1) */
static size_t cache_cap = 0;     /* number of cached entries */

/* Ensure that a(1)..a(n) are stored in cache.  n>=1. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap) return;         /* already computed */

    size_t old_cap = cache_cap;
    size_t new_cap = (size_t)n;

    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache == NULL) exit(1);
        /* initialize known initial terms */
        for (size_t i = 0; i < 13 && i < new_cap; ++i)
            cache[i] = init_vals[i];
        for (size_t i = 13; i < new_cap; ++i)
            cache[i] = (seq_t)0;
    } else if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i)
            cache[i] = (seq_t)0;
    }

    cache_cap = new_cap;

    /* Compute new terms only for indices >= 14. */
    idx_t start = (idx_t)(old_cap >= 13 ? old_cap : 13);
    if (start > n - 1) return;   /* nothing new to compute */

    for (idx_t i = (start + 1 > 13 ? start + 1 : 14); i <= n; ++i) {
        seq_t val = cache[i - 2];
        val += (seq_t)3 * cache[i - 3];
        val += (seq_t)6 * cache[i - 4];
        val += (seq_t)3 * cache[i - 5];
        val -= cache[i - 6];
        val -= (seq_t)5 * cache[i - 7];
        val -= (seq_t)4 * cache[i - 8];
        val -= (seq_t)4 * cache[i - 9];
        val += (seq_t)2 * cache[i - 11];
        cache[i - 1] = val;
    }
}

seq_t a(idx_t n)
{
    if (n <= 13) return init_vals[n - 1];
    ensure_cache(n);
    return cache[n - 1];
}
