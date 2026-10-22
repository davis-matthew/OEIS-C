/*
 * A307203  Coordination sequence for trivalent node of type alpha'' in the
 *          first Moore pentagonal tiling.
 * https://oeis.org/A307203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) + a(n-4) - a(n-5) + a(n-6) -
 *            a(n-7) for n > 16, with initial values a(0)=1, a(1)=3, a(2)=7,
 *            a(3)=10, a(4)=14, a(5)=21, a(6)=26, a(7)=32, a(8)=38, a(9)=46,
 *            a(10)=51, a(11)=56, a(12)=61, a(13)=71, a(14)=73, a(15)=78,
 *            a(16)=84
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The implementation works for GNU C11, Clang C11, and most compilers
 * with C++17. For C++, a std::vector is used to store the cache; for C,
 * a malloced array is used. No external library dependencies are needed.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Numerator coefficients of the generating function for n = 0..16 */
static const long long NUM[17] = {
    1,  2,  5,  5,  7,  8,  4,  8,
    3,  4,  2,  0, -1,  1, -4,  2, -2
};

/* Denominator coefficients of the generating function */
static const int Q[8] = {
    1, -1,  1, -1, -1,  1, -1,  1
};

#ifndef __cplusplus
/* C implementation: cache via malloc/realloc */
static seq_t *seq_cache = NULL;
static idx_t seq_cache_size = 0;
#else
/* C++ implementation: cache via std::vector */
static std::vector<seq_t> seq_cache;
#endif

seq_t a(idx_t n)
{
#ifndef __cplusplus
    /* C version */
    if (n >= seq_cache_size) {
        idx_t old_size = seq_cache_size;
        idx_t new_size = n + 1;

        /* Reallocate memory for the cache */
        seq_t *new_cache = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure (undefined) */
        seq_cache = new_cache;

        /* Compute new elements */
        for (idx_t i = old_size; i <= n; ++i) {
            if (i <= 16) {
                seq_t sum = 0;
                idx_t maxj = (i < 7) ? i : 7;
                for (idx_t j = 1; j <= maxj; ++j) {
                    sum += (seq_t)Q[j] * seq_cache[i - j];
                }
                seq_cache[i] = (seq_t)NUM[(int)i] - sum;
            } else {
                seq_cache[i] = seq_cache[i - 1] - seq_cache[i - 2]
                              + seq_cache[i - 3] + seq_cache[i - 4]
                              - seq_cache[i - 5] + seq_cache[i - 6]
                              - seq_cache[i - 7];
            }
        }
        seq_cache_size = new_size;
    }
    return seq_cache[n];
#else
    /* C++ version */
    size_t new_size = (size_t)n + 1;
    if (seq_cache.size() < new_size) {
        size_t old_size = seq_cache.size();
        seq_cache.resize(new_size);
        for (size_t i = old_size; i <= n; ++i) {
            if (i <= 16) {
                seq_t sum = 0;
                size_t maxj = (i < 7) ? i : 7;
                for (size_t j = 1; j <= maxj; ++j) {
                    sum += (seq_t)Q[j] * seq_cache[i - j];
                }
                seq_cache[i] = (seq_t)NUM[(int)i] - sum;
            } else {
                seq_cache[i] = seq_cache[i - 1] - seq_cache[i - 2]
                               + seq_cache[i - 3] + seq_cache[i - 4]
                               - seq_cache[i - 5] + seq_cache[i - 6]
                               - seq_cache[i - 7];
            }
        }
    }
    return seq_cache[(size_t)n];
#endif
}
