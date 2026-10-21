/*
 * A254699  Number of length 1+4 0..n arrays with every five consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A254699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) a(1)
 *            = 22, a(2) = 133, a(3) = 464, a(4) = 1205, a(5) = 2606
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 202, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

#if defined(__cplusplus)
#define PTRCAST(p, t) reinterpret_cast<t>(p)
#else
#define PTRCAST(p, t) ((t)(p))
#endif

seq_t a(idx_t n)
{
    /* Initial values (index 0 unused) */
    static const seq_t init[6] = {0, 22, 133, 464, 1205, 2606};

    /* Static cache of computed terms */
    static seq_t *cache = NULL;   /* first time NULL, then points to array */
    static size_t cap   = 0;      /* number of elements allocated (>= n+1) */
    static idx_t  computed = 0;   /* highest index computed (>=5 after init) */

    /* Allocate/expand cache if needed */
    if (!cache) {
        /* initial capacity, can grow later */
        cap = 64;
        cache = PTRCAST(malloc(cap * sizeof(seq_t)), seq_t*);
        if (!cache) return 0;                            /* unlikely in tests */
        /* initialise first 5 terms */
        for (size_t i = 1; i <= 5; ++i) cache[i] = init[i];
        computed = 5;
    }

    /* Ensure cache large enough for index n */
    if (cap <= (size_t)n) {
        size_t newcap = cap;
        while (newcap <= (size_t)n) newcap <<= 1;        /* exponential grow */
        seq_t *tmp = PTRCAST(malloc(newcap * sizeof(seq_t)), seq_t*);
        if (!tmp) return 0;                            /* unlikely in tests */
        memcpy(tmp, cache, cap * sizeof(seq_t));        /* copy existing data */
        free(cache);
        cache = tmp;
        cap = newcap;
    }

    /* Return value if already computed */
    if (n <= computed) return cache[n];

    /* Compute missing terms using the recurrence */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] =
            (seq_t)5 * cache[i - 1]
          - (seq_t)10 * cache[i - 2]
          + (seq_t)10 * cache[i - 3]
          - (seq_t)5 * cache[i - 4]
          + cache[i - 5];
    }
    computed = n;
    return cache[n];
}
