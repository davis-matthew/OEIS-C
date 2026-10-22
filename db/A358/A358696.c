/*
 * A358696  Number of self-avoiding closed paths in the 5 X n grid graph which
 *          pass through all vertices on four (left, right, upper, lower)
 *          sides of the graph.
 * https://oeis.org/A358696
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + a(n-2) - 20*a(n-3) + 37*a(n-4) + 16*a(n-5) * -
 *            12*a(n-6) - 7*a(n-7) + 5*a(n-8) - a(n-9) for n > 10 * Initial
 *            values (offset = 2): a(2)=1, a(3)=5, a(4)=36, a(5)=191,
 *            a(6)=1123, * a(7)=6410, a(8)=37165, a(9)=214515, a(10)=1240200
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;          /* number of allocated slots, >= n+1 */

/* initial values for indices 0..10 (indices 0 and 1 unused) */
static const seq_t init_vals[11] = {
    0, 0,                          /* a(0), a(1) unused */
    1, 5, 36, 191, 1123, 6410,     /* a(2)–a(7)   */
    37165, 214515, 1240200          /* a(8)–a(10)  */
};

/* ensure that cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;                                 /* already enough */

    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache;

#ifdef __cplusplus
    new_cache = new seq_t[new_cap];              /* C++: allocate with constructors */
#else
    new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_cache)
        exit(1);                                 /* allocation failure */

    /* copy existing values */
    if (cache) {
        size_t i;
        for (i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n-th term (n >= 2).  Memoises computed values. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                                 /* undefined for n < 2 */

    /* initialise cache on first call */
    if (!cache) {
        ensure_cache(10);                         /* indices 0..10 needed initially */
        /* copy initial values */
        size_t i;
        for (i = 0; i <= 10; ++i)
            cache[i] = init_vals[i];
    }

    ensure_cache(n);

    /* compute missing terms up to n */
    for (idx_t i = 11; i <= n; ++i) {
        seq_t val = 6 * cache[i - 1] + cache[i - 2];
        val -= 20 * cache[i - 3];
        val += 37 * cache[i - 4] + 16 * cache[i - 5];
        val -= 12 * cache[i - 6];
        val -= 7 * cache[i - 7];
        val += 5 * cache[i - 8];
        val -= cache[i - 9];
        cache[i] = val;
    }

    return cache[n];
}
