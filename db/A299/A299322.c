/*
 * A299322  Ranks of {2,3}-power towers with neither consecutive 2's nor
 *          consecutive 3's; see Comments.
 * https://oeis.org/A299322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-3)+2*a(n-4)-2*a(n-5) for n>=10 Initial
 *            values: a(1)=1, a(2)=2, a(3)=4, a(4)=5, a(5)=10, a(6)=11,
 *            a(7)=22, a(8)=23, a(9)=45
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* static cache with 1‑indexing; cache[0] unused */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated size of cache   */
    static size_t max_computed = 0; /* largest n already filled */
    static bool initialized = false;

    /* first call: allocate and initialise constants up to n=9 */
    if (!initialized) {
        /* start with room for at least 10 entries (index 0..9) */
        size_t init_cap = 16;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        assert(cache);                     /* abort if allocation fails */
        cap = init_cap;
        /* initialise constant values */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)10;
        cache[6] = (seq_t)11;
        cache[7] = (seq_t)22;
        cache[8] = (seq_t)23;
        cache[9] = (seq_t)45;
        max_computed = 9;
        initialized = true;
    }

    /* enlarge cache if necessary to hold up to n */
    if ((size_t)n > cap - 1) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        assert(tmp);                       /* abort if allocation fails */
        cache = tmp;
        cap = new_cap;
    }

    /* if requested term already computed, return it */
    if ((size_t)n <= max_computed) {
        return cache[(size_t)n];
    }

    /* compute missing terms up to n using the recurrence */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        if (i >= 10) {
            /* recurrence: a(i)=a(i-1)+a(i-2)-a(i-3)+2*a(i-4)-2*a(i-5) */
            seq_t term = cache[i-1] + cache[i-2] - cache[i-3]
                         + (seq_t)2 * cache[i-4]
                         - (seq_t)2 * cache[i-5];
            cache[i] = term;
        } else {
            /* for safety: constants already set, this branch shouldn't fire */
            /* but we leave it, no action */
        }
    }
    max_computed = (size_t)n;
    return cache[(size_t)n];
}
