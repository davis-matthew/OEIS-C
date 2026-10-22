/*
 * A315227  Coordination sequence Gal.5.169.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-7) - a(n-14) for n > 14, with initial values
 *            a(0)=1, a(1)=6, a(2)=10, a(3)=14, a(4)=22, a(5)=20, a(6)=30,
 *            a(7)=36, a(8)=42, a(9)=40, a(10)=50, a(11)=58, a(12)=50,
 *            a(13)=66, a(14)=72
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* pointer to allocated cache */
static size_t cached = 0;            /* current size of allocated cache */
static size_t stored = 0;            /* number of entries already computed */

static void ensure_capacity(size_t needed)
{
    if (cached >= needed)
        return;
    size_t new_cap = cached ? cached * 2 : 128;
    while (new_cap <= needed)
        new_cap <<= 1;

#ifdef __cplusplus
    seq_t *new_arr = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < cached; ++i)
            new_arr[i] = cache[i];
        delete[] cache;
    }
#else
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr && new_cap > 0)
        abort();
#endif
    cache = new_arr;
    cached = new_cap;
}

static const seq_t init_vals[15] = {
    1, 6, 10, 14, 22, 20, 30,
    36, 42, 40, 50, 58, 50, 66, 72
};

seq_t a(idx_t n)
{
    /* Ensure cache is large enough for at least n+1 elements */
    ensure_capacity((size_t)n + 1);

    /* Initialize the first 15 terms on first use or if not yet done */
    if (stored == 0)
    {
        for (size_t i = 0; i < 15; ++i)
            cache[i] = init_vals[i];
        stored = 15;
    }

    /* If the requested value is already computed, return it */
    if ((size_t)n < stored)
        return cache[n];

    /* Compute missing entries using the recurrence */
    for (size_t i = stored; i <= (size_t)n; ++i)
    {
        if (i > 14)
        {
            /* recurrence is valid for i > 14 */
            cache[i] = 2 * cache[i - 7] - cache[i - 14];
        }
        else
        {
            /* Just copy the static initial values for indices < 15 */
            cache[i] = init_vals[i];
        }
    }
    stored = (size_t)n + 1;   /* newly computed up to n inclusive */

    return cache[n];
}
