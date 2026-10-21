/*
 * A254959  Squares not divisible by 10 with digits d_1, d_2, ... d_k such
 *          that d_1^2 + ... + d_k^2 is a square.
 * https://oeis.org/A254959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isqrt_seq, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based: squares not divisible by 10 whose digit‑square sum is a square. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/isqrt_seq.h"
#include "../utils/seq_realloc.h"

static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;
static idx_t next_sqrt = 1;

static void ensure_cache_capacity(void)
{
    if (cache_size >= cache_cap) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 1;
        seq_t *tmp = seq_realloc(seq_cache, cache_cap, new_cap);
        if (!tmp) abort();
        seq_cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    while (cache_size < n) {
        seq_t sq = (seq_t)next_sqrt * next_sqrt;
        if (sq % 10 != 0) {
            seq_t tmp = sq;
            seq_t digit_sum = 0;
            while (tmp > 0) {
                unsigned int d = (unsigned int)(tmp % 10);
                digit_sum += (seq_t)d * (seq_t)d;
                tmp /= 10;
            }
            seq_t r = isqrt_seq(digit_sum);
            if (r * r == digit_sum) {
                ensure_cache_capacity();
                seq_cache[cache_size++] = sq;
            }
        }
        ++next_sqrt;
    }
    return seq_cache[n - 1];
}
