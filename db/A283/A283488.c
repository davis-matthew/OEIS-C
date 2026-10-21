/*
 * A283488  Number of n X 2 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, vertical and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 3*a(n-2) - 16*a(n-3) - 13*a(n-4) - 10*a(n-5) -
 *            13*a(n-6) - 8*a(n-7) - 3*a(n-8) - 2*a(n-9) - a(n-10) Initial
 *            terms: a(1)=0, a(2)=0, a(3)=2, a(4)=12, a(5)=64, a(6)=312,
 *            a(7)=1460, a(8)=6624, a(9)=29394, a(10)=128264
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"
#ifdef __cplusplus
#include <type_traits>
#endif

static const seq_t init_vals[11] = {0, 0, 0, 2, 12, 64, 312, 1460, 6624, 29394, 128264};
static const int coeffs[10] = {6, -3, -16, -13, -10, -13, -8, -3, -2, -1};

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Return initial values directly to avoid allocation for very small n */
    if (n <= 10)
        return init_vals[n];

    /* Allocate cache on first use or enlarge if necessary */
    if (cache == NULL || cache_cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_arr = alloc_seq_array(new_cap);
        /* Zero-initialize (alloc_seq_array may already do this) */
        for (size_t k = 0; k < new_cap; ++k)
            new_arr[k] = 0;
        if (cache != NULL) {
            for (size_t k = 0; k < cache_cap; ++k)
                new_arr[k] = cache[k];
            seq_free_array(cache);
        }
        cache = new_arr;
        cache_cap = new_cap;
    }

    /* Store initial terms if not already present */
    if (cache_len < 10) {
        for (idx_t i = 1; i <= 10; ++i)
            cache[i] = init_vals[i];
        cache_len = 10;
    }

    /* Compute new terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
#ifdef __cplusplus
        if (std::is_fundamental<seq_t>::value) {
            unsigned __int128 sum = (unsigned __int128)cache[i - 1] * 6;
            for (int j = 1; j < 10; ++j) {
                idx_t idx = i - 1 - j;
                unsigned __int128 term = (unsigned __int128)cache[idx] *
                                         ((coeffs[j] > 0) ? coeffs[j] : -coeffs[j]);
                if (coeffs[j] > 0)
                    sum += term;
                else
                    sum -= term;
            }
            cache[i] = (seq_t)sum;
        } else {
            seq_t sum = cache[i - 1] * 6;
            for (int j = 1; j < 10; ++j) {
                idx_t idx = i - 1 - j;
                seq_t mul = (seq_t)((coeffs[j] > 0) ? coeffs[j] : -coeffs[j]);
                seq_t term = cache[idx] * mul;
                if (coeffs[j] > 0)
                    sum += term;
                else
                    sum -= term;
            }
            cache[i] = sum;
        }
#else
        unsigned __int128 sum = (unsigned __int128)cache[i - 1] * 6;
        for (int j = 1; j < 10; ++j) {
            idx_t idx = i - 1 - j;
            unsigned __int128 term = (unsigned __int128)cache[idx] *
                                     ((coeffs[j] > 0) ? coeffs[j] : -coeffs[j]);
            if (coeffs[j] > 0)
                sum += term;
            else
                sum -= term;
        }
        cache[i] = (seq_t)sum;
#endif
    }

    cache_len = n;
    return cache[n];
}
