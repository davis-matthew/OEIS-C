/*
 * A267914  Number of 3Xn 0..2 arrays with every repeated value in every row
 *          unequal to the previous repeated value, and in every column equal
 *          to the previous repeated value, and new values introduced in
 *          row-major sequential order.
 * https://oeis.org/A267914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Sequence a(n) satisfies the linear recurrence
 *
 *   a(n) = 31*a(n-1)
 *        - 26*a(n-2)
 *        - 6768*a(n-3)
 *        + 39456*a(n-4)
 *        + 479232*a(n-5)
 *        - 3704832*a(n-6)
 *        - 12386304*a(n-7)
 *        + 115867648*a(n-8)
 *        + 83886080*a(n-9)
 *        - 1073741824*a(n-10)
 *
 * for n > 12, with the first 12 terms
 * (a(1)=5, a(2)=122, a(3)=2304, …, a(12)=464405881704372).
 * The implementation memoises computed values so it works in any call order.
 */

#include <stdlib.h>

/* First 12 terms of the sequence. */
static const seq_t ITERS[12] = {
    5ULL,
    122ULL,
    2304ULL,
    47916ULL,
    878004ULL,
    16435188ULL,
    292341636ULL,
    5209312500ULL,
    90756319716ULL,
    1576811812500ULL,
    27101734404228ULL,
    464405881704372ULL
};

seq_t a(idx_t n)
{
    /* static cache of already computed terms (1‑based index) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* allocated capacity, > n */

    if (n <= 12)
        return ITERS[n - 1];

    /* initialise cache on first use */
    if (!cache) {
        cache_cap = 13; /* indices 0..12; 0 unused */
        cache   = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache)
            return (seq_t)0; /* out‑of‑memory – unlikely in tests */
        for (idx_t i = 1; i <= 12; ++i)
            cache[i] = ITERS[i - 1];
    }

    /* grow cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;          /* double until big enough */

        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp)
            return (seq_t)0;        /* out‑of‑memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = 13; i <= n; ++i) {
        seq_t v = 0;
        v += cache[i - 1] * (seq_t)31;
        v -= cache[i - 2] * (seq_t)26;
        v -= cache[i - 3] * (seq_t)6768;
        v += cache[i - 4] * (seq_t)39456;
        v += cache[i - 5] * (seq_t)479232;
        v -= cache[i - 6] * (seq_t)3704832;
        v -= cache[i - 7] * (seq_t)12386304;
        v += cache[i - 8] * (seq_t)115867648;
        v += cache[i - 9] * (seq_t)83886080;
        v -= cache[i -10] * (seq_t)1073741824;
        cache[i] = v;
    }

    return cache[n];
}
