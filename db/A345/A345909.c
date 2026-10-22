/*
 * A345909  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has alternating sum 1.
 * https://oeis.org/A345909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 4707, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth positive integer k whose
   composition in standard order has alternating sum 1. */

#include <stdlib.h>

static inline int alt_sum_is_one(idx_t k) {
    long long S = 0;
    int sign_bit = -1;          /* (-1)^1 = -1 for the first difference      */
    int prev_pos = 0;
    int t = 0;
    int bitpos = 1;
    idx_t temp = k;

    while (temp) {
        if (temp & 1) {
            int diff = bitpos - prev_pos;
            S += diff * sign_bit;
            sign_bit = -sign_bit;
            prev_pos = bitpos;
            t++;
        }
        temp >>= 1;
        bitpos++;
    }
    if (t & 1)  /* t odd, multiply by (-1) */
        S = -S;
    return S == 1;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t last_k = 0;

    if (n == 0)
        return 0;   /* Not used; offset is 1 */

    while (cache_len < n) {
        last_k++;
        if (alt_sum_is_one(last_k)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_k;
        }
    }
    return cache[n - 1];
}
