/*
 * A331827  Positive negabinary-Niven numbers k (A331728) such that -k is a
 *          negative negabinary-Niven number (A331819).
 * https://oeis.org/A331827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 7248, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   negabinary_digit_sum_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate sequence by scanning integers k ≥ 1, selecting those
   for which k is divisible by the sums of digits of k and of –k
   in their negabinary representations. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/negabinary_digit_sum_ll.h"

/* dynamic array helper */
static void append_value(seq_t val, seq_t **cache, size_t *len, size_t *cap)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *cache = tmp;
        *cap = new_cap;
    }
    (*cache)[*len] = val;
    ++*len;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_candidate = 1ULL;

    if (n == 0) return 0;

    while (cache_len < (size_t)n) {
        seq_t k = (seq_t)next_candidate++;
        long long k_ll = (long long)k;

        unsigned long long s_pos = negabinary_digit_sum_ll(k_ll);
        unsigned long long s_neg = negabinary_digit_sum_ll(-k_ll);

        if (s_pos != 0 && s_neg != 0 &&
            (k % (seq_t)s_pos == 0) &&
            (k % (seq_t)s_neg == 0))
        {
            append_value(k, &cache, &cache_len, &cache_cap);
        }
    }
    return cache[n - 1];
}
