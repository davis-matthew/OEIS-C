/*
 * A275374  Numbers k that divide sigma(k*(k+1)/2).
 * https://oeis.org/A275374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k that divide σ(k·(k+1)/2) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {          /* offset 1 – behaviour for n<1 does not matter */
        return (seq_t)0;
    }

#ifdef __cplusplus
    /* C++ implementation – use std::vector to manage construction/destruction of
       arbitrary‑precision objects. */
    static std::vector<seq_t> cache;
    static idx_t next_k = 1;

    while (cache.size() < (size_t)n) {
        idx_t k     = next_k++;
        unsigned __int128 prod = (unsigned __int128)k * (k + 1);
        idx_t m     = (idx_t)(prod / 2);

        seq_t sigma = sigma_of(m);
        seq_t k_s   = (seq_t)k;

        if (sigma % k_s == 0) {
            cache.push_back(k_s);
        }
    }

    return cache[(size_t)n - 1];
#else
    /* C implementation – allocated with realloc, works with plain integral seq_t. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1;

    while (cache_len < (size_t)n) {
        idx_t k     = next_k++;
        unsigned __int128 prod = (unsigned __int128)k * (k + 1);
        idx_t m     = (idx_t)(prod / 2);

        seq_t sigma = sigma_of(m);
        seq_t k_s   = (seq_t)k;

        if (sigma % k_s == 0) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache   = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k_s;
        }
    }

    return cache[(size_t)n - 1];
#endif
}
