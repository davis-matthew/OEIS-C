/*
 * A250855  Number of (3+1) X (n+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 34, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: recurrence a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4)
 *          + 21*a(n-5) - 7*a(n-6) + a(n-7)    (n>7)
 * Initial values:
 *   a(1) = 2670, a(2) = 13097, a(3) = 44797, a(4) = 123016,
 *   a(5) = 290646, a(6) = 614965, a(7) = 1195457
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#if defined(__cplusplus)
#include <type_traits>
#endif

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    /* Prefer C++ type traits to distinguish 64‑bit unsigned builtin from
       arbitrary‑precision types.  The 64‑bit case uses unsigned __int128
       for safe intermediate arithmetic. */
    if constexpr (std::is_same_v<seq_t, unsigned long long>) {
        static seq_t *cache = nullptr;
        static idx_t cache_cap = 0;
        static idx_t cache_len = 0;

        if (!cache) {
            cache_cap = 16;
            cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
            memset(cache, 0, sizeof(seq_t) * cache_cap);
            cache_len = 0;
        }
        if (n >= cache_cap) {
            idx_t new_cap = cache_cap;
            while (n >= new_cap) new_cap <<= 1;
            seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            memset(new_cache + cache_cap, 0, sizeof(seq_t) * (new_cap - cache_cap));
            cache = new_cache;
            cache_cap = new_cap;
        }
        if (n > cache_len) {
            static const seq_t init[7] = {
                (seq_t)2670, (seq_t)13097, (seq_t)44797,
                (seq_t)123016, (seq_t)290646, (seq_t)614965,
                (seq_t)1195457
            };
            for (idx_t i = cache_len + 1; i <= n; ++i) {
                seq_t val;
                if (i <= 7) {
                    val = init[i - 1];
                } else {
                    unsigned __int128 v = (unsigned __int128)cache[i-1] * 7
                                        - (unsigned __int128)cache[i-2] * 21
                                        + (unsigned __int128)cache[i-3] * 35
                                        - (unsigned __int128)cache[i-4] * 35
                                        + (unsigned __int128)cache[i-5] * 21
                                        - (unsigned __int128)cache[i-6] * 7
                                        + (unsigned __int128)cache[i-7];
                    val = (seq_t)v;
                }
                cache[i] = val;
            }
            cache_len = n;
        }
        return cache[n];
    } else {
        /* Arbitrary‑precision implementation – use seq_t operators only. */
        static seq_t *cache = nullptr;
        static idx_t cache_cap = 0;
        static idx_t cache_len = 0;

        if (!cache) {
            cache_cap = 16;
            cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
            memset(cache, 0, sizeof(seq_t) * cache_cap);
            cache_len = 0;
        }
        if (n >= cache_cap) {
            idx_t new_cap = cache_cap;
            while (n >= new_cap) new_cap <<= 1;
            seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            memset(new_cache + cache_cap, 0, sizeof(seq_t) * (new_cap - cache_cap));
            cache = new_cache;
            cache_cap = new_cap;
        }
        if (n > cache_len) {
            static const seq_t init[7] = {
                (seq_t)2670, (seq_t)13097, (seq_t)44797,
                (seq_t)123016, (seq_t)290646, (seq_t)614965,
                (seq_t)1195457
            };
            for (idx_t i = cache_len + 1; i <= n; ++i) {
                seq_t val;
                if (i <= 7) {
                    val = init[i - 1];
                } else {
                    val = 7 * cache[i-1];
                    val -= 21 * cache[i-2];
                    val += 35 * cache[i-3];
                    val -= 35 * cache[i-4];
                    val += 21 * cache[i-5];
                    val -= 7 * cache[i-6];
                    val += cache[i-7];
                }
                cache[i] = val;
            }
            cache_len = n;
        }
        return cache[n];
    }
#else
    /* C implementation – works for all integer seq_t sizes.  For a
       builtin unsigned 64‑bit integer we still guard against overflow
       by using unsigned __int128. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        memset(cache, 0, sizeof(seq_t) * cache_cap);
        cache_len = 0;
    }
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (n >= new_cap) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        memset(new_cache + cache_cap, 0, sizeof(seq_t) * (new_cap - cache_cap));
        cache = new_cache;
        cache_cap = new_cap;
    }
    if (n > cache_len) {
        static const seq_t init[7] = {
            (seq_t)2670, (seq_t)13097, (seq_t)44797,
            (seq_t)123016, (seq_t)290646, (seq_t)614965,
            (seq_t)1195457
        };
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val;
            if (i <= 7) {
                val = init[i - 1];
            } else {
                unsigned __int128 v = (unsigned __int128)cache[i-1] * 7
                                    - (unsigned __int128)cache[i-2] * 21
                                    + (unsigned __int128)cache[i-3] * 35
                                    - (unsigned __int128)cache[i-4] * 35
                                    + (unsigned __int128)cache[i-5] * 21
                                    - (unsigned __int128)cache[i-6] * 7
                                    + (unsigned __int128)cache[i-7];
                val = (seq_t)v;
            }
            cache[i] = val;
        }
        cache_len = n;
    }
    return cache[n];
#endif
}
