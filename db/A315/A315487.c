/*
 * A315487  Coordination sequence Gal.5.214.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-13) - a(n-14) (n > 14) Initial values:
 *            a(0)=1, a(1)=6, a(2)=11, a(3)=16, a(4)=22, a(5)=27, a(6)=31,
 *            a(7)=41, a(8)=45, a(9)=50, a(10)=56, a(11)=61, a(12)=66,
 *            a(13)=72, a(14)=78
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static caching to avoid recomputation. */
    static seq_t *cache   = NULL;   /* array of computed terms          */
    static size_t cache_cap = 0;    /* allocated capacity (elements)    */
    static size_t cache_size = 0;   /* highest index computed (>=0)     */

    /* Initial capacity: enough for n up to 10000;  grow if needed. */
    const size_t INITIAL_CAP = 10001;

    if (cache == NULL) {                          /* first call */
        cache_cap = INITIAL_CAP;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                     /* allocation failure */

        /* Fill the first 15 terms */
        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)6;
        cache[2]  = (seq_t)11;
        cache[3]  = (seq_t)16;
        cache[4]  = (seq_t)22;
        cache[5]  = (seq_t)27;
        cache[6]  = (seq_t)31;
        cache[7]  = (seq_t)41;
        cache[8]  = (seq_t)45;
        cache[9]  = (seq_t)50;
        cache[10] = (seq_t)56;
        cache[11] = (seq_t)61;
        cache[12] = (seq_t)66;
        cache[13] = (seq_t)72;
        cache[14] = (seq_t)78;
        cache_size = 14;                          /* highest index set so far */
    }

    /* If the requested index is already known, return it. */
    if ((idx_t)n <= cache_size)
        return cache[(size_t)n];

    /* Ensure capacity for the requested index. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
        /* For n > 14: a(n) = a(n-1) + a(n-13) - a(n-14) */
        cache[i] = cache[i - 1]          /* a(i-1)   */
                 + cache[i - 13]         /* a(i-13)  */
                 - cache[i - 14];        /* a(i-14)  */
    }
    cache_size = (size_t)n;
    return cache[(size_t)n];
}
