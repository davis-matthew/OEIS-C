/*
 * A286986  Number of connected dominating sets in the n-antiprism graph.
 * https://oeis.org/A286986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) - a(n-4), a(1) = 3, a(2)
 *            = 15, a(3) = 54, a(4) = 175 This implementation memoises
 *            computed values. It works for both C11 and C++17 builds. For C++
 *            the storage is a std::vector, for C the storage is a
 *            malloc‑based array
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#if defined(__cplusplus)
#include <vector>
static std::vector<seq_t> a_cache;   /* a_cache[0] is unused */
#else
static seq_t *a_cache = NULL;
static size_t cache_len = 0;          /* one past the highest cached n */
#endif

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0; /* undefined for n < 1, but safe */
    }

#if defined(__cplusplus)
    /* Initialise cache with the first four terms. */
    if (a_cache.size() < 5) {
        a_cache.resize(5);
        a_cache[0] = (seq_t)0; /* unused */
        a_cache[1] = (seq_t)3;
        a_cache[2] = (seq_t)15;
        a_cache[3] = (seq_t)54;
        a_cache[4] = (seq_t)175;
    }

    /* Extend cache to include n. */
    while (a_cache.size() <= n) {
        idx_t i = (idx_t)a_cache.size();          /* next index to compute */
        seq_t val = 6 * a_cache[i-1]              /* 6*a(n-1)   */
                  - 11 * a_cache[i-2]             /* -11*a(n-2) */
                  + 6 * a_cache[i-3]              /* +6*a(n-3)  */
                  - a_cache[i-4];                 /* -a(n-4)    */
        a_cache.push_back(val);
    }

    return a_cache[n];
#else
    /* C implementation */
    if (cache_len == 0) {
        cache_len = 5;                            /* indices 0..4 */
        a_cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (a_cache == NULL) abort();
        a_cache[0] = (seq_t)0; /* unused */
        a_cache[1] = (seq_t)3;
        a_cache[2] = (seq_t)15;
        a_cache[3] = (seq_t)54;
        a_cache[4] = (seq_t)175;
    }

    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;            /* one past n */
        seq_t *tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
        if (tmp == NULL) abort();
        a_cache = tmp;
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            a_cache[i] = (seq_t)(6 * a_cache[i-1]
                               - 11 * a_cache[i-2]
                               + 6 * a_cache[i-3]
                               - a_cache[i-4]);
        }
        cache_len = new_len;
    }

    return a_cache[n];
#endif
}
