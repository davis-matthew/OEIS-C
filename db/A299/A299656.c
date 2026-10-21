/*
 * A299656  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4, 5 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) +3*a(n-2) -6*a(n-3) -18*a(n-4) -15*a(n-5)
 *            -17*a(n-6) -32*a(n-7) -12*a(n-8) Initial values: a(1)=4,
 *            a(2)=32, a(3)=227, a(4)=1642, a(5)=11888, a(6)=86123,
 *            a(7)=624007, a(8)=4521433
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t current_max = 0;

/* Ensure the cache can hold indices up to [cap-1] */
static void ensure_capacity(size_t cap)
{
    if (cap <= cache_cap) return;

#if __cplusplus
    seq_t *newp = new seq_t[cap];
    if (cache) {
        for (size_t i = 1; i <= current_max; ++i) newp[i] = cache[i];
        delete [] cache;
    }
    cache = newp;
#else
    seq_t *newp = (seq_t*)realloc(cache, cap * sizeof(seq_t));
    cache = newp;
#endif
    cache_cap = cap;
}

seq_t a(idx_t n)
{
    static const seq_t init[9] = {0, 4, 32, 227, 1642, 11888, 86123, 624007, 4521433};

    if (n <= 8) return init[n];

    /* reserve space up to n */
    ensure_capacity((size_t)n + 1);

    /* initialise first eight terms if not yet done */
    if (current_max < 8) {
        for (size_t i = 1; i <= 8; ++i) cache[i] = init[i];
        current_max = 8;
    }

    /* compute missing terms up to n */
    for (size_t i = current_max + 1; i <= (size_t)n; ++i) {
        seq_t val = 7 * cache[i-1] + 3 * cache[i-2];
        val -= 6 * cache[i-3];
        val -= 18 * cache[i-4];
        val -= 15 * cache[i-5];
        val -= 17 * cache[i-6];
        val -= 32 * cache[i-7];
        val -= 12 * cache[i-8];
        cache[i] = val;
    }
    current_max = (size_t)n;
    return cache[(size_t)n];
}
