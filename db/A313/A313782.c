/*
 * A313782  Coordination sequence Gal.5.289.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313782
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

/*   A313782: Coordination sequence Gal.5.289.1
    Recurrence (valid for n > 10):
        a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4)
               + 2*a(n-5) - 2*a(n-6) + 2*a(n-7) - 2*a(n-8)
               + 2*a(n-9) - a(n-10)
    Initial values:
        a(0)=1, a(1)=5, a(2)=10, a(3)=15, a(4)=21, a(5)=26,
        a(6)=31, a(7)=37, a(8)=42, a(9)=47, a(10)=52
*/
#include <stdlib.h>
#include <string.h>   /* for memcpy in C mode */

seq_t a(idx_t n)
{
    /* Initial values for n = 0..10 */
    static const seq_t init[11] = {
        (seq_t)1,  (seq_t)5,  (seq_t)10, (seq_t)15, (seq_t)21,
        (seq_t)26, (seq_t)31, (seq_t)37, (seq_t)42, (seq_t)47,
        (seq_t)52
    };

    /* Static cache for memoisation */
    static seq_t *cache   = NULL;  /* array of a[0..last] */
    static idx_t  last    = (idx_t)-1; /* largest index computed so far */
    static size_t cap      = 0;   /* current allocated size */

    /* For small n, just return the known values */
    if (n <= 10)
        return init[(size_t)n];

    /* Lazily allocate and initialise cache if this is the first call */
    if (cache == NULL) {
        /* Ensure we have room for at least n+1 entries */
        size_t need = (size_t)n + 1;
        if (need < 11) need = 11;
        cap = need;

#ifdef __cplusplus
        cache = new seq_t[cap];
#else
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
#endif
        if (!cache) abort();          /* out of memory */

        /* Copy the initial 11 values into the cache */
        for (size_t i = 0; i <= 10; ++i)
            cache[i] = init[i];

        last = 10;
    }

    /* If the requested value is already computed, return it */
    if ((idx_t)n <= last)
        return cache[(size_t)n];

    /* Grow the cache array if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        cap = new_cap;

#ifdef __cplusplus
        seq_t *tmp = new seq_t[cap];
        /* copy existing values */
        for (size_t i = 0; i <= (size_t)last; ++i)
            tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        seq_t *tmp = (seq_t*)realloc(cache, cap * sizeof(seq_t));
        if (!tmp) abort();  /* out of memory */
        cache = tmp;
#endif
    }

    /* Compute terms from last+1 up to n (inclusive) */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[(size_t)i] =
            2 * cache[(size_t)i-1]
          - 2 * cache[(size_t)i-2]
          + 2 * cache[(size_t)i-3]
          - 2 * cache[(size_t)i-4]
          + 2 * cache[(size_t)i-5]
          - 2 * cache[(size_t)i-6]
          + 2 * cache[(size_t)i-7]
          - 2 * cache[(size_t)i-8]
          + 2 * cache[(size_t)i-9]
          - cache[(size_t)i-10];
    }
    last = n;
    return cache[(size_t)n];
}
