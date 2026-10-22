/*
 * A377011  a(n) = Sum_{k=0..n} 3^k * binomial(2*n+1,n-k).
 * https://oeis.org/A377011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binomial(2*n+1,n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/grow_seq_array.h"
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_len   = 0;

    if (n < cache_len)
        return cache[n];

    size_t need = (size_t)n + 1;
    if (need > cache_size)
        grow_seq_array(&cache, &cache_size, need);

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1) {
            val = (seq_t)6;
        } else {
#ifdef __cplusplus
            if constexpr (std::is_integral_v<seq_t>) {
                /* integer arithmetic – use 128‑bit intermediates */
                uint64_t f1 = 28ull * (uint64_t)i - 6ull;
                uint64_t f2 = 64ull * (uint64_t)i - 32ull;
                unsigned __int128 mul1 = (unsigned __int128)cache[i-1] * f1;
                unsigned __int128 mul2 = (unsigned __int128)cache[i-2] * f2;
                unsigned __int128 numer = mul1 - mul2;
                unsigned __int128 denom = 3ull * (unsigned __int128)i;
                val = (seq_t)(numer / denom);
            } else {
                /* arbitrary‑precision type – use ordinary operators */
                seq_t t1 = cache[i-1] * (seq_t)(28ull * (uint64_t)i - 6ull);
                seq_t t2 = cache[i-2] * (seq_t)(64ull * (uint64_t)i - 32ull);
                seq_t numer = t1 - t2;
                val = numer / (seq_t)(3ull * (uint64_t)i);
            }
#else
            uint64_t f1 = 28ull * (uint64_t)i - 6ull;
            uint64_t f2 = 64ull * (uint64_t)i - 32ull;
            unsigned __int128 mul1 = (unsigned __int128)cache[i-1] * f1;
            unsigned __int128 mul2 = (unsigned __int128)cache[i-2] * f2;
            unsigned __int128 numer = mul1 - mul2;
            unsigned __int128 denom = 3ull * (unsigned __int128)i;
            val = (seq_t)(numer / denom);
#endif
        }
        cache[i] = val;
    }

    cache_len = need;
    return cache[n];
}
