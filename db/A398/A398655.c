/*
 * A398655  Lower (1, 2/3) midsequence of triangular numbers (A000217) and
 *          tetrahedral numbers (A000330); see Comments.
 * https://oeis.org/A398655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A398655 – Lower (1, 2/3) midsequence of triangular and tetrahedral numbers.
   Recurrence (order 10):
   a(n) = 4*a(n-1) - 6*a(n-2) + 3*a(n-3) + 3*a(n-4) - 6*a(n-5) + 3*a(n-6)
          + 3*a(n-7) - 6*a(n-8) + 4*a(n-9) - a(n-10)
   initial values: a(0)=0, a(1)=1, a(2)=6, a(3)=15, a(4)=30, a(5)=51,
                   a(6)=81, a(7)=121, a(8)=172, a(9)=235.
*/

#include <stdlib.h>
#include <stdint.h>

static const seq_t init10[10] = {
    0ULL, 1ULL, 6ULL, 15ULL, 30ULL,
    51ULL, 81ULL, 121ULL, 172ULL, 235ULL
};

extern seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ build: vector of seq_t holds values and resizes as needed */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(10);
        for (size_t i = 0; i < 10; ++i) cache[i] = init10[i];
    }
    if (n >= cache.size()) {
        size_t oldsz = cache.size();
        cache.resize(n + 1);
        for (idx_t i = oldsz; i <= n; ++i) {
            /* recurrence is valid only for i >= 10, but oldsz is at least 10 */
            seq_t pos = cache[i - 1] * 4
                      + cache[i - 3] * 3
                      + cache[i - 4] * 3
                      + cache[i - 6] * 3
                      + cache[i - 7] * 3
                      + cache[i - 9] * 4;
            seq_t neg = cache[i - 2] * 6
                      + cache[i - 5] * 6
                      + cache[i - 8] * 6
                      + cache[i -10];
            cache[i] = pos - neg;
        }
    }
    return cache[(size_t)n];
#else
    /* C build: raw array with memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* ensure capacity */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure */

        /* initialise first 10 terms after any reallocation */
        for (idx_t i = 0; i < 10 && i < new_size; ++i) {
            tmp[i] = init10[i];
        }

        /* compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 10) continue;        /* already initialised */
            seq_t pos = tmp[i - 1] * 4
                      + tmp[i - 3] * 3
                      + tmp[i - 4] * 3
                      + tmp[i - 6] * 3
                      + tmp[i - 7] * 3
                      + tmp[i - 9] * 4;
            seq_t neg = tmp[i - 2] * 6
                      + tmp[i - 5] * 6
                      + tmp[i - 8] * 6
                      + tmp[i -10];
            tmp[i] = pos - neg;
        }

        cache = tmp;
        cache_size = new_size;
    }
    return cache[(size_t)n];
#endif
}
