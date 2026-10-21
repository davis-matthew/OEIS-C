/*
 * A251793  Numbers n such that the sum of the triangular numbers T(n) and
 *          T(n+1) is equal to the sum of the octagonal numbers N(m) and
 *          N(m+1) for some m.
 * https://oeis.org/A251793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+98*a(n-2)-98*a(n-3)-a(n-4)+a(n-5) Initial terms :
 *            a(1)=0, a(2)=2, a(3)=68, a(4)=286, a(5)=6760
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc, abort */

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

#ifdef __cplusplus
    /* C++ implementation using std::vector for correct construction of
       arbitrary‑precision seq_t objects. */
    static std::vector<seq_t> cache;
    static bool init = false;
    if (!init) {
        cache.resize(6);          /* indices 0..5 (0 unused) */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)68;
        cache[4] = (seq_t)286;
        cache[5] = (seq_t)6760;
        init = true;
    }
    if (n >= cache.size()) {
        idx_t old = cache.size();
        cache.resize(n + 1);
        for (idx_t i = old; i <= n; ++i) {
            if (i <= 5) continue;                      /* base entries already set */
            cache[i] = cache[i-1] + (seq_t)98 * cache[i-2]
                        - (seq_t)98 * cache[i-3] - cache[i-4] + cache[i-5];
        }
    }
    return cache[n];
#else
    /* C implementation using raw array (seq_t must be trivially
       constructible). */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;
    if (cache == NULL) {
        size_t cap = 6;               /* indices 0..5 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)68;
        cache[4] = (seq_t)286;
        cache[5] = (seq_t)6760;
        cache_max = 5;
    }
    if (n > cache_max) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i <= 5) continue;  /* base entries already set */
            cache[i] = cache[i-1] + (seq_t)98 * cache[i-2]
                        - (seq_t)98 * cache[i-3] - cache[i-4] + cache[i-5];
        }
        cache_max = n;
    }
    return cache[n];
#endif
}
