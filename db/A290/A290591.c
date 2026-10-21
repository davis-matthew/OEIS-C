/*
 * A290591  Number of irredundant sets in the n-web graph.
 * https://oeis.org/A290591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1)+3*a(n-2)+4*a(n-3)+22*a(n-4)+12*a(n-5)
 *            -164*a(n-6)+80*a(n-7)-24*a(n-8)+80*a(n-9), n>9 Initial values:
 *            a(1)=4, a(2)=22, a(3)=112, a(4)=618, a(5)=3044, a(6)=14026,
 *            a(7)=70340, a(8)=347074, a(9)=1713820
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t* alloc_cache(size_t cap) {
#if defined(__cplusplus)
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}
static void free_cache(seq_t* p, size_t /*cap*/) {
#if defined(__cplusplus)
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    /* Static cache: index 0 is unused, indices 1..9 contain the base values. */
    static seq_t* cache = NULL;
    static size_t cache_cap = 0;      /* total allocated entries (including index 0) */
    static size_t cache_len = 0;      /* largest index that has a computed value */

    /* --- first‑time initialization ------------------------------------- */
    if (!cache) {
        cache_cap = 10;                          /* needs indices 0..9 at least */
        cache = alloc_cache(cache_cap);
        if (!cache) return (seq_t)0;             /* allocation failure */

        /* base terms (as seq_t constants) */
        cache[0] = 0;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)22;
        cache[3] = (seq_t)112;
        cache[4] = (seq_t)618;
        cache[5] = (seq_t)3044;
        cache[6] = (seq_t)14026;
        cache[7] = (seq_t)70340;
        cache[8] = (seq_t)347074;
        cache[9] = (seq_t)1713820;
        cache_len = 9;
    }

    if ((size_t)n == 0) return (seq_t)0;          /* undefined for n=0 */

    /* If the term is already cached, return it directly. */
    if ((size_t)n <= cache_len)
        return cache[n];

    /* --- enlarge cache if necessary ----------------------------------- */
    if ((size_t)n + 1 > cache_cap) {              /* need at least n+1 slots (0..n) */
        size_t new_cap = cache_cap ? cache_cap : 10;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;                       /* double until large enough */

        seq_t* tmp = alloc_cache(new_cap);
        if (!tmp) return (seq_t)0;               /* allocation failure */

        /* Copy existing values to the new array. */
        for (size_t i = 0; i <= cache_len; ++i)
            tmp[i] = cache[i];

        free_cache(cache, cache_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* --- compute missing terms up to n -------------------------------- */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i)
    {
        seq_t v1 = cache[i-1];
        seq_t v2 = cache[i-2];
        seq_t v3 = cache[i-3];
        seq_t v4 = cache[i-4];
        seq_t v5 = cache[i-5];
        seq_t v6 = cache[i-6];
        seq_t v7 = cache[i-7];
        seq_t v8 = cache[i-8];
        seq_t v9 = cache[i-9];

        seq_t val = (seq_t)4 * v1
                  + (seq_t)3 * v2
                  + (seq_t)4 * v3
                  + (seq_t)22 * v4
                  + (seq_t)12 * v5
                  - (seq_t)164 * v6
                  + (seq_t)80 * v7
                  - (seq_t)24 * v8
                  + (seq_t)80 * v9;

        cache[i] = val;
    }
    cache_len = (size_t)n;
    return cache[n];
}
