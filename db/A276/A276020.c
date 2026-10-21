/*
 * A276020  n^2 * a(n) = 2*(17*n^2-21*n+9) * a(n-1) - 4*(112*n^2-280*n+197) *
 *          a(n-2) + 40*(68*n^2-256*n+251) * a(n-3) - 1600*(2*n-5)^2 * a(n-4),
 *          with a(0)=1, a(1)=10, a(2)=90, a(3)=780.
 * https://oeis.org/A276020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   n^2*a(n) = 2*(17*n^2-21*n+9)*a(n-1)-4*(112*n^2-280*n+197)*a(n-2)
 *            * +40*(68*n^2-256*n+251)*a(n-3)-1600*(2*n-5)^2*a(n-4) * a(0)=1,
 *            a(1)=10, a(2)=90, a(3)=780
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Static cache for memoization */
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)90;
        cache[3] = (seq_t)780;
    }
#else
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)90;
        cache[3] = (seq_t)780;
        cache_size = 4;
    }
#endif

    /* Ensure cache covers index n */
#ifdef __cplusplus
    if (n >= cache.size()) {
        size_t needed = (size_t)n + 1;
        cache.reserve(needed);
        for (size_t i = cache.size(); i < needed; ++i) {
            idx_t ii = i;
            seq_t coef1 = (seq_t)2 * ((seq_t)17*ii*ii - (seq_t)21*ii + (seq_t)9);
            seq_t coef2 = (seq_t)4 * ((seq_t)112*ii*ii - (seq_t)280*ii + (seq_t)197);
            seq_t coef3 = (seq_t)40 * ((seq_t)68*ii*ii - (seq_t)256*ii + (seq_t)251);
            seq_t base4 = (seq_t)(2*ii - 5);
            seq_t coef4 = (seq_t)1600 * base4 * base4;
            seq_t denom = (seq_t)ii * (seq_t)ii;
            seq_t val;
            if (sizeof(seq_t) <= 8) {
                __int128 t1 = (__int128)coef1 * (__int128)cache[i-1];
                __int128 t2 = (__int128)coef2 * (__int128)cache[i-2];
                __int128 t3 = (__int128)coef3 * (__int128)cache[i-3];
                __int128 t4 = (__int128)coef4 * (__int128)cache[i-4];
                __int128 num = t1 - t2 + t3 - t4;
                val = (seq_t)(num / (__int128)denom);
            } else {
                val = ((coef1 * cache[i-1])
                       - (coef2 * cache[i-2])
                       + (coef3 * cache[i-3])
                       - (coef4 * cache[i-4])) / denom;
            }
            cache.push_back(val);
        }
    }
    return cache[n];
#else
    if ((size_t)n >= cache_size) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            while (new_cap < needed) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;
            cache = tmp;
            cache_cap = new_cap;
        }
        for (size_t i = cache_size; i < needed; ++i) {
            idx_t ii = i;
            seq_t coef1 = (seq_t)2 * ((seq_t)17*ii*ii - (seq_t)21*ii + (seq_t)9);
            seq_t coef2 = (seq_t)4 * ((seq_t)112*ii*ii - (seq_t)280*ii + (seq_t)197);
            seq_t coef3 = (seq_t)40 * ((seq_t)68*ii*ii - (seq_t)256*ii + (seq_t)251);
            seq_t base4 = (seq_t)(2*ii - 5);
            seq_t coef4 = (seq_t)1600 * base4 * base4;
            seq_t denom = (seq_t)ii * (seq_t)ii;
            seq_t val;
            if (sizeof(seq_t) <= 8) {
                __int128 t1 = (__int128)coef1 * (__int128)cache[i-1];
                __int128 t2 = (__int128)coef2 * (__int128)cache[i-2];
                __int128 t3 = (__int128)coef3 * (__int128)cache[i-3];
                __int128 t4 = (__int128)coef4 * (__int128)cache[i-4];
                __int128 num = t1 - t2 + t3 - t4;
                val = (seq_t)(num / (__int128)denom);
            } else {
                val = ((coef1 * cache[i-1])
                       - (coef2 * cache[i-2])
                       + (coef3 * cache[i-3])
                       - (coef4 * cache[i-4])) / denom;
            }
            cache[i] = val;
        }
        cache_size = needed;
    }
    return cache[n];
#endif
}
