/*
 * A311647  Coordination sequence Gal.4.85.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3) + a(n-5) - a(n-8) for n > 10 with initial values
 *            a(0)=1, a(1)=4, a(2)=8, a(3)=12, a(4)=19, a(5)=22, a(6)=25,
 *            a(7)=32, a(8)=35, a(9)=39, a(10)=45
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

/* Memoized sequence array */
static seq_t *cache = NULL;          /* pointer to sequence of values   */
static size_t cache_len = 0;         /* number of elements stored       */
static size_t cache_cap = 0;         /* allocated capacity (in elements)*/

/* Ensure the cache has at least `need` capacity */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    while (newcap < need)
        newcap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    assert(tmp);                     /* abort on allocation failure   */
    cache = tmp;
    cache_cap = newcap;
}

/* Compute a(n) for the coordination sequence A311647 */
seq_t a(idx_t n)
{
    /* If we have enough precomputed values, just return the one we need */
    if (cache_len > 0 && (size_t)n < cache_len)
        return cache[(size_t)n];

    /* Initialise the initial terms once */
    if (cache_len == 0) {
        ensure_capacity(11);
        static const seq_t base[11] = {
            (seq_t)1, (seq_t)4, (seq_t)8, (seq_t)12, (seq_t)19,
            (seq_t)22, (seq_t)25, (seq_t)32, (seq_t)35, (seq_t)39,
            (seq_t)45
        };
        for (size_t i = 0; i < 11; ++i)
            cache[i] = base[i];
        cache_len = 11;
    }

    /* If the requested index is still within the precomputed range, return */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Grow the cache if necessary */
    ensure_capacity((size_t)n + 1);

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* For i <= 10 the value is already known from base initialization,
           but the loop starts at cache_len which is >= 11 after init. */
        assert(i >= 11); /* safety: we are only here for i >= 11 */

        /* a(i) = a(i-3) + a(i-5) - a(i-8) */
        seq_t val = cache[i - 3] + cache[i - 5];
        val -= cache[i - 8];
        cache[i] = val;
    }

    /* Update the length of the cache */
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
