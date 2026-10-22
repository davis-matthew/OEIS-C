/*
 * A328781  Nonnegative integers k such that k and k^2 contain the same number
 *          of zero digits in their decimal expansion.
 * https://oeis.org/A328781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th nonnegative integer k such that k and k^2 have
 *            the same number of zero digits in their decimal representations.
 *            a(1) = 0
 * Range:     u8 n <= 15, u16 n <= 610, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_counts.h"

/* Return the number of decimal zero digits of x.  It uses the helper
   digit_counts from utils. */
static inline unsigned char count_zeros_seq(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    return cnt[0];
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 0;

    /* Expand the cache until it contains at least n terms. */
    while (cache_len < n) {
        seq_t k = (seq_t)next_candidate;
        unsigned char zeros_k  = count_zeros_seq(k);
        seq_t k2 = k * k;                 /* sqaure in seq_t */
        unsigned char zeros_k2 = count_zeros_seq(k2);

        if (zeros_k == zeros_k2) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
