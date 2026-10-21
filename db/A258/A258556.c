/*
 * A258556  Number of (3+1) X (n+1) 0..1 arrays with every 2 X 2 subblock
 *          ne-sw antidiagonal difference nondecreasing horizontally and nw+se
 *          diagonal sum nondecreasing vertically.
 * https://oeis.org/A258556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence: a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) +
 *            a(n-4) - 3*a(n-5) + 3*a(n-6) - a(n-7) for n > 10 Initial values:
 *            a(1)=104, a(2)=296, a(3)=598, a(4)=985, a(5)=1403, a(6)=1917,
 *            a(7)=2542, a(8)=3222, a(9)=3940, a(10)=4767
 * Range:     u8 n <= 1, u16 n <= 35, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/*
 * Compute the nth term of OEIS sequence A258556.
 *
 * a(1) is the first term in the list below.
 * The function works for any n >= 1 in any order of calls.
 * Results are cached in a static array/vector, so subsequent
 * calls are O(1) per new term.
 */
seq_t a(idx_t n)
{
    /* dummy entry for 0‑based array indexing; real data start at 1 */
    static const seq_t init[11] = {
        0,          /* index 0 unused */
        104, 296, 598, 985, 1403,
        1917, 2542, 3222, 3940, 4767
    };

    if (n == 0) return (seq_t)0;  /* out‑of‑bounds requests are harmless */

    /* fast path for the first ten values */
    if (n <= 10) return init[(size_t)n];

#ifdef __cplusplus
    /* C++: use std::vector to support arbitrary‑precision seq_t classes */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(11);               /* indices 0..10 */
        for (size_t i = 0; i <= 10; ++i)
            cache[i] = init[i];
    }

    /* remember old size before reallocation */
    size_t old_size = cache.size();
    if (old_size <= (size_t)n) {
        cache.resize((size_t)n + 1);    /* indices 0..n */
    }

    for (size_t i = old_size; i <= (size_t)n; ++i) {
        /* recurrence uses terms i-1 .. i-7 ; they are already set */
        seq_t v = (seq_t)3 * cache[i-1];
        v -= (seq_t)3 * cache[i-2];
        v += cache[i-3];
        v += cache[i-4];
        v -= (seq_t)3 * cache[i-5];
        v += (seq_t)3 * cache[i-6];
        v -= cache[i-7];
        cache[i] = v;
    }
    return cache[(size_t)n];
#else
    /* C: manage a raw array via malloc/realloc */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* number of allocated slots (indices 0..size-1) */

    if (cache == NULL) {
        cache_size = 11;               /* indices 0..10 */
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure */
        for (idx_t i = 0; i <= 10; ++i)
            cache[i] = init[i];
    }

    idx_t old_size = cache_size;
    if (cache_size <= n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    for (idx_t i = old_size; i <= n; ++i) {
        /* recurrence uses terms i-1 .. i-7 ; they are already set */
        seq_t v = (seq_t)3 * cache[i-1];
        v -= (seq_t)3 * cache[i-2];
        v += cache[i-3];
        v += cache[i-4];
        v -= (seq_t)3 * cache[i-5];
        v += (seq_t)3 * cache[i-6];
        v -= cache[i-7];
        cache[i] = v;
    }
    return cache[(size_t)n];
#endif
}
