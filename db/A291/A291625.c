/*
 * A291625  Numbers k such that 0 is the smallest decimal digit of k^2.
 * https://oeis.org/A291625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that k^2 contains decimal digit 0
   (OEIS A291625).  The function returns the n‑th such k (1‑based).  */
#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#include <stdbool.h>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector for cache */
    static std::vector<seq_t> cache;
    static idx_t max_k_checked = 0;

    if (n <= cache.size())
        return cache[n-1];

    for (idx_t k = max_k_checked + 1; ; ++k) {
        seq_t sq = (seq_t)k * (seq_t)k;
        bool has_zero = false;
        while (sq > 0 && !has_zero) {
            if (sq % 10 == 0)
                has_zero = true;
            else
                sq /= 10;
        }
        if (has_zero) {
            cache.push_back((seq_t)k);
            if (cache.size() == n) {
                max_k_checked = k;
                return cache[n-1];
            }
        }
    }
#else
    /* C implementation using dynamic array */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t cache_len = 0;
    static idx_t max_k_checked = 0;

    if (n <= cache_len)
        return cache[n-1];

    for (idx_t k = max_k_checked + 1; ; ++k) {
        seq_t sq = (seq_t)k * (seq_t)k;
        bool has_zero = false;
        while (sq > 0 && !has_zero) {
            if (sq % 10 == 0)
                has_zero = true;
            else
                sq /= 10;
        }
        if (has_zero) {
            if (cache_len >= cap) {
                size_t new_cap = cap ? cap * 2 : 8;
                seq_t *newp = realloc(cache, new_cap * sizeof(seq_t));
                if (!newp) exit(1);
                cache = newp;
                cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
            if (cache_len == n) {
                max_k_checked = k;
                return cache[n-1];
            }
        }
    }
#endif
}
