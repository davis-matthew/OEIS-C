/*
 * A352006  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/2)} 2^k * a(k) *
 *          a(n-2*k-1).
 * https://oeis.org/A352006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊(n-1)/2⌋} 2^k * a(k) * a(n-2k-1), a(0)=1
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 28, u64 n <= 55, u128 n <= 108,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache_vec;

seq_t a(idx_t n)
{
    if (cache_vec.size() <= n) {
        size_t old = cache_vec.size();
        cache_vec.resize(n + 1);
        for (idx_t i = old; i <= n; ++i) {
            if (i == 0) {
                cache_vec[0] = (seq_t)1;
                continue;
            }
            seq_t sum = (seq_t)0;
            idx_t limit = (i - 1) / 2;
            for (idx_t k = 0; k <= limit; ++k) {
                seq_t term = pow2(k);
                term *= cache_vec[k];
                term *= cache_vec[i - 2 * k - 1];
                sum += term;
            }
            cache_vec[i] = sum;
        }
    }
    return cache_vec[n];
}

#else
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t computed = 0;   /* next index to be computed */

seq_t a(idx_t n)
{
    if (n >= cache_size) {
        idx_t new_size = cache_size == 0 ? 1 : cache_size;
        while (new_size <= n) new_size <<= 1;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_size);
        if (cache) {
            memcpy(new_cache, cache, sizeof(seq_t) * cache_size);
            free(cache);
        }
        cache = new_cache;
        cache_size = new_size;
    }

    if (n >= computed) {
        for (idx_t i = computed; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else {
                seq_t sum = (seq_t)0;
                idx_t limit = (i - 1) / 2;
                for (idx_t k = 0; k <= limit; ++k) {
                    seq_t term = pow2(k);
                    term *= cache[k];
                    term *= cache[i - 2 * k - 1];
                    sum += term;
                }
                cache[i] = sum;
            }
        }
        computed = n + 1;
    }
    return cache[n];
}
#endif
