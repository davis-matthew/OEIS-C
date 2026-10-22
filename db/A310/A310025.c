/*
 * A310025  Coordination sequence Gal.4.31.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - 3*a(n-2) + 4*a(n-3) - 5*a(n-4) + 6*a(n-5)
   - 7*a(n-6) + 8*a(n-7) - 7*a(n-8) + 6*a(n-9) - 5*a(n-10)
   + 4*a(n-11) - 3*a(n-12) + 2*a(n-13) - a(n-14)  for n >= 17
   Initial values: a(0)=1, a(1)=3, a(2)=5, a(3)=8, a(4)=10,
   a(5)=15, a(6)=18, a(7)=19, a(8)=22, a(9)=25,
   a(10)=28, a(11)=31, a(12)=34, a(13)=38,
   a(14)=39, a(15)=41, a(16)=46.
*/
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Helper for casting malloced memory in a way that compiles in both C
   and C++. */
#ifdef __cplusplus
#define PTRCAST(x) static_cast<seq_t*>(x)
#else
#define PTRCAST(x) ((seq_t*)(x))
#endif

seq_t a(idx_t n) {
    /* Static initial terms.  The cast ensures that the integer
       constants are converted to seq_t, calling the appropriate
       constructor when seq_t is a C++ class. */
    static const seq_t init[17] = {
        (seq_t)1,  (seq_t)3,  (seq_t)5,  (seq_t)8,  (seq_t)10,
        (seq_t)15, (seq_t)18, (seq_t)19, (seq_t)22, (seq_t)25,
        (seq_t)28, (seq_t)31, (seq_t)34, (seq_t)38, (seq_t)39,
        (seq_t)41, (seq_t)46
    };

    /* If the requested index is in the initial block, return it. */
    if (n < 17) return init[(size_t)n];

    /* Lazy, expandable cache for computed terms. */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;   /* allocated size of cache        */
    static size_t last    = 0;   /* highest index that has been
                                    computed and stored in cache   */

    /* Allocate and initialise cache on first call. */
    if (cache == NULL) {
        /* Start with room for at least the initial 17 terms. */
        cap = 64;
        cache = PTRCAST(malloc(cap * sizeof(seq_t)));
        if (!cache) abort();      /* unrecoverable failure */

#ifdef __cplusplus
        /* Construct the objects in the array so that they are
           ready for assignment even when seq_t is a C++ class. */
        for (size_t i = 0; i < cap; ++i) {
            ::new(&cache[i]) seq_t();
        }
#endif
        /* Copy the known initial terms into the cache. */
        for (size_t i = 0; i < 17; ++i) {
            cache[i] = init[i];
        }
        last = 16;
    }

    /* Grow the cache if necessary. */
    size_t target = (size_t)n;
    if (target >= cap) {
        size_t newcap = cap;
        /* Double until the new capacity is sufficient. */
        while (target >= newcap) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();          /* unrecoverable failure */
        cache = tmp;
        cap = newcap;
#ifdef __cplusplus
        /* Construct the newly added elements. */
        for (size_t i = 17; i < cap; ++i) {
            ::new(&cache[i]) seq_t();
        }
#endif
    }

    /* Compute terms up to the requested index. */
    for (size_t i = last + 1; i <= target; ++i) {
        seq_t val =
              (seq_t)2 * cache[i-1]
            - (seq_t)3 * cache[i-2]
            + (seq_t)4 * cache[i-3]
            - (seq_t)5 * cache[i-4]
            + (seq_t)6 * cache[i-5]
            - (seq_t)7 * cache[i-6]
            + (seq_t)8 * cache[i-7]
            - (seq_t)7 * cache[i-8]
            + (seq_t)6 * cache[i-9]
            - (seq_t)5 * cache[i-10]
            + (seq_t)4 * cache[i-11]
            - (seq_t)3 * cache[i-12]
            + (seq_t)2 * cache[i-13]
            - cache[i-14];
        cache[i] = val;
    }
    last = target;
    return cache[(size_t)n];
}
