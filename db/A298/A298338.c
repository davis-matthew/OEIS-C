/*
 * A298338  a(n) = a(n-1) + a(n-2) + a([n/2]), where a(0) = 1, a(1) = 1, a(2)
 *          = 1.
 * https://oeis.org/A298338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a([n/2]); a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Handle the known base cases directly. */
    if (n == 0 || n == 1 || n == 2)
        return (seq_t)1;

#ifdef __cplusplus
    /* C++: use a std::vector for automatic construction of seq_t objects. */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(3);
        cache[0] = cache[1] = cache[2] = (seq_t)1;
    }
    if (n >= cache.size()) {
        cache.resize(n + 1);
        for (size_t i = 3; i <= (size_t)n; ++i) {
            cache[i] = cache[i-1] + cache[i-2] + cache[i/2];
        }
    }
    return cache[(size_t)n];
#else
    /* C: use a raw array allocated with malloc/realloc. */
    static seq_t *cache = NULL;
    static size_t sz = 0;     /* one past the highest computed index */
    static size_t cap = 0;    /* allocated capacity */

    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;        /* allocation failed */
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        sz = 3;
    }

    if ((idx_t)n >= (idx_t)sz) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cap = new_cap;
        for (size_t i = sz; i <= (size_t)n; ++i) {
            cache[i] = cache[i-1] + cache[i-2] + cache[i/2];
        }
        sz = (size_t)n + 1;
    }

    return cache[(size_t)n];
#endif
}
