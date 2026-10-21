/*
 * A251126  Number of (n+1) X (6+1) 0..1 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements greater than the maximum of its
 *          antidiagonal elements.
 * https://oeis.org/A251126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5) (n>6)
 *            a(1)=212, a(2)=315, a(3)=430, a(4)=612, a(5)=854, a(6)=1214
 *            Offset: 1
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 38, u64 n <= 84, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#   include <vector>
#endif

/* ---- memoised recurrence implementation ---- */
seq_t a(idx_t n)
{
    /* Base values for indices 1..6, index 0 unused */
    static const seq_t base_vals[7] = {0, 212, 315, 430, 612, 854, 1214};

    /* Handle the trivial case */
    if (n == 0) return 0;

#ifdef __cplusplus
    /* C++ path: use std::vector for storage */
    static std::vector<seq_t> cache;
    static idx_t last_n = 0;
    /* initial capacity */
    if (cache.empty()) {
        cache.resize(7 + 1);            /* indices 0..6 */
        for (idx_t i = 1; i <= 6; ++i) cache[i] = base_vals[i];
        last_n = 6;
    }
    /* resize if necessary */
    if (n + 1 > cache.size()) cache.resize(n + 1);
    /* compute new terms */
    for (idx_t cur = last_n + 1; cur <= n; ++cur) {
        if (cur <= 6) {
            cache[cur] = base_vals[cur];
        } else {
            cache[cur] =
                4 * cache[cur-1]
              - 5 * cache[cur-2]
              + cache[cur-3]
              + 2 * cache[cur-4]
              - cache[cur-5];
        }
    }
    last_n = n;
    return cache[n];

#else
    /* C path: use raw memory allocation */
    static seq_t *cache = 0;
    static size_t cache_size = 0;   /* number of allocated slots (0..size-1) */
    static idx_t last_n = 0;
    /* initialise */
    if (cache == 0) {
        cache_size = 7 + 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == 0) return 0;    /* allocation failure -> 0 */
        for (idx_t i = 1; i <= 6; ++i) cache[i] = base_vals[i];
        last_n = 6;
    }
    /* ensure space */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == 0) return 0;      /* allocation failure -> 0 */
        cache = tmp;
        cache_size = new_size;
    }
    /* compute missing entries */
    for (idx_t cur = last_n + 1; cur <= n; ++cur) {
        if (cur <= 6) {
            cache[cur] = base_vals[cur];
        } else {
            cache[cur] =
                4 * cache[cur-1]
              - 5 * cache[cur-2]
              + cache[cur-3]
              + 2 * cache[cur-4]
              - cache[cur-5];
        }
    }
    last_n = n;
    return cache[n];
#endif
}
