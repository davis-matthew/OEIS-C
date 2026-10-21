/*
 * A252978  Number of n X 3 nonnegative integer arrays with upper left 0 and
 *          lower right its king-move distance away minus 2 and every value
 *          increasing by 0 or 1 with every step right, diagonally se or down.
 * https://oeis.org/A252978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n>7 Initial values:
 *            a(1)=1, a(2)=1, a(3)=1, a(4)=33, a(5)=266, a(6)=851, a(7)=1836
 * Range:     u8 n <= 4, u16 n <= 22, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* C implementation -------------------------------------------------------- */
#ifndef __cplusplus
static seq_t *cache_c = NULL;
static size_t cache_len_c = 0;
static size_t cache_cap_c = 0;

static void ensure_cache_capacity_c(idx_t n)
{
    if (cache_cap_c < (size_t)n) {
        size_t new_cap = cache_cap_c ? cache_cap_c : 4;
        while (new_cap < (size_t)n) new_cap <<= 1;
        if (!cache_c)
            cache_c = (seq_t*)malloc(sizeof(seq_t) * new_cap);
        else
            cache_c = (seq_t*)realloc(cache_c, sizeof(seq_t) * new_cap);
        cache_cap_c = new_cap;
    }
}
#endif

/* C++ implementation ----------------------------------------------------- */
#ifdef __cplusplus
static std::vector<seq_t> cache_cpp;
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for the offset, safe to return 0 */

#if defined(__cplusplus)
    if (cache_cpp.size() < (size_t)n) {
        const seq_t init[8] = {1, 1, 1, 33, 266, 851, 1836};
        for (size_t i = cache_cpp.size(); i < (size_t)n; ++i) {
            seq_t val;
            if (i < 7)
                val = init[i];
            else
                val = cache_cpp[i - 1] * 3
                      - cache_cpp[i - 2] * 3
                      + cache_cpp[i - 3];
            cache_cpp.push_back(val);
        }
    }
    return cache_cpp[(size_t)(n - 1)];
#else
    ensure_cache_capacity_c(n);
    if (cache_len_c < (size_t)n) {
        const seq_t init[8] = {1, 1, 1, 33, 266, 851, 1836};
        for (idx_t k = cache_len_c + 1; k <= n; ++k) {
            seq_t val;
            if (k <= 7)
                val = init[(size_t)k - 1];
            else
                val = cache_c[(size_t)(k - 2)] * 3
                      - cache_c[(size_t)(k - 3)] * 3
                      + cache_c[(size_t)(k - 4)];
            cache_c[(size_t)(k - 1)] = val;
        }
        cache_len_c = n;
    }
    return cache_c[(size_t)(n - 1)];
#endif
}
