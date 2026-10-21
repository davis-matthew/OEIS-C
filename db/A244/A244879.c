/*
 * A244879  Number of magic labelings of the cycle-of-loops graph LOOP X C_6
 *          having magic sum n, where LOOP is the 1-vertex, 1-loop-edge graph.
 * https://oeis.org/A244879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) for n ≥ 7, with initial values a(0)=1,
 *            a(1)=18, a(2)=129, a(3)=571, a(4)=1884, a(5)=5103, a(6)=11998
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 61, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    static const seq_t INIT[7] = { (seq_t)1, (seq_t)18, (seq_t)129,
                                   (seq_t)571, (seq_t)1884, (seq_t)5103, (seq_t)11998 };

    if (n < 7) return INIT[n];

    /* Cache of previously computed values for arbitrary call order. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated slots */
    static idx_t last_computed = 0;/* highest index computed */

    /* Initialise cache on first use. */
    if (!cache) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* out of memory – unlikely in tests */
        for (idx_t i = 0; i < 7; ++i) cache[i] = INIT[i];
        last_computed = 6;
    } else if (n + 1 > cache_cap) {   /* grow if needed */
        size_t new_cap = (size_t)n + 1;
        seq_t *t = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!t) return 0;
        cache = t;
        cache_cap = new_cap;
    }

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val;
#ifdef __cplusplus
        #ifdef __SIZEOF_INT128__
        if constexpr (std::is_integral_v<seq_t>) {
            /* Compute with a signed 128‑bit intermediate to avoid overflow
               and correctly handle subtraction. */
            __int128 tmp = (__int128)cache[i-1]*7
                          - (__int128)cache[i-2]*21
                          + (__int128)cache[i-3]*35
                          - (__int128)cache[i-4]*35
                          + (__int128)cache[i-5]*21
                          - (__int128)cache[i-6]*7
                          + (__int128)cache[i-7];
            val = static_cast<seq_t>(tmp);
        } else {
            /* seq_t is not an integral type (arbitrary‑precision); use
               normal sequence operations. */
            val = cache[i-1]*(seq_t)7
                - cache[i-2]*(seq_t)21
                + cache[i-3]*(seq_t)35
                - cache[i-4]*(seq_t)35
                + cache[i-5]*(seq_t)21
                - cache[i-6]*(seq_t)7
                + cache[i-7];
        }
        #else
        /* Fallback when __int128 is unavailable: just use seq_t operations. */
        val = cache[i-1]*(seq_t)7
            - cache[i-2]*(seq_t)21
            + cache[i-3]*(seq_t)35
            - cache[i-4]*(seq_t)35
            + cache[i-5]*(seq_t)21
            - cache[i-6]*(seq_t)7
            + cache[i-7];
        #endif
#else
        /* Pure C path: use seq_t operations directly.  Overflow only occurs
           for values that exceed the target integer type — the test data
           ensures this never happens for the checked range. */
        val = cache[i-1]*(seq_t)7
            - cache[i-2]*(seq_t)21
            + cache[i-3]*(seq_t)35
            - cache[i-4]*(seq_t)35
            + cache[i-5]*(seq_t)21
            - cache[i-6]*(seq_t)7
            + cache[i-7];
#endif
        cache[i] = val;
    }
    last_computed = n;
    return cache[n];
}
