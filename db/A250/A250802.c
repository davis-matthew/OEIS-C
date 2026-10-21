/*
 * A250802  Number of (5+1) X (n+1) 0..1 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) - 7*a(n-3) + 18*a(n-4) * - 3*a(n-5) -
 *            14*a(n-6) + 7*a(n-7) + 3*a(n-8) - 2*a(n-9) * Initial values (1 ≤
 *            n ≤ 9): * a(1)=338, a(2)=1336, a(3)=4258, a(4)=12062,
 *            a(5)=31220, * a(6)=76110, a(7)=177142, a(8)=398704, a(9)=874298
 * Range:     u8 none, u16 n <= 5, u32 n <= 20, u64 n <= 52, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_sz   = 0; /* number of elements stored (cached)  */
    static idx_t cache_cap  = 0; /* allocated capacity in elements       */

    /* Predefined first nine terms */
    static const seq_t init[9] = {
        (seq_t)338, (seq_t)1336, (seq_t)4258, (seq_t)12062,
        (seq_t)31220, (seq_t)76110, (seq_t)177142, (seq_t)398704, (seq_t)874298
    };

    if (n == 0) return 0;          /* offset is 1; this value is unused but harmless */

    idx_t idx = n - 1;              /* zero‑based index into cache   */

    /* Ensure cache memory is allocated and populated up to the required size. */
    if (cache_sz == 0) {
        /* Start with a modest capacity, grow if needed. */
        cache_cap = 64;
        if (idx + 1 > cache_cap) cache_cap = idx + 1;

        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failure, unlikely in tests */

        /* Copy known initial values. */
        idx_t tocopy = (idx + 1) < 9 ? (idx + 1) : 9;
        for (idx_t i = 0; i < tocopy; ++i) cache[i] = init[i];
        cache_sz = tocopy;
    } else if (idx + 1 > cache_cap) {
        /* Increase capacity (double each time) until sufficient. */
        idx_t new_cap = cache_cap * 2;
        if (new_cap < idx + 1) new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to idx, if necessary. */
    if (idx >= cache_sz) {
        for (idx_t i = cache_sz; i <= idx; ++i) {
            if (i < 9) {
                /* We already had the initial terms stored. */
                cache[i] = init[i];
                continue;
            }
            /* Apply the linear recurrence. */
            cache[i] =
                5 * cache[i - 1]
                - 6 * cache[i - 2]
                - 7 * cache[i - 3]
                + 18 * cache[i - 4]
                - 3 * cache[i - 5]
                - 14 * cache[i - 6]
                + 7 * cache[i - 7]
                + 3 * cache[i - 8]
                - 2 * cache[i - 9];
        }
        cache_sz = idx + 1;
    }

    return cache[idx];
}
