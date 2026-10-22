/*
 * A339076  Numbers which are coprime to their digital sum (A007953).
 * https://oeis.org/A339076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A339076: numbers coprime to their digital sum. Computed by incremental search. */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/digit_sum.h"

static seq_t *seq_cache = NULL;
static size_t seq_cache_cap = 0;
static size_t seq_cache_len = 0;
static idx_t next_k = 1;

static void ensure_cache_capacity(size_t needed)
{
    if (seq_cache_cap < needed) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 64;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = new_cache;
        seq_cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (seq_cache_len < (size_t)n) {
        seq_t k_seq = (seq_t)next_k;
        seq_t sum = digit_sum(k_seq);
        if (gcd(k_seq, sum) == 1) {
            ensure_cache_capacity(seq_cache_len + 1);
            seq_cache[seq_cache_len++] = k_seq;
        }
        ++next_k;
    }
    return seq_cache[(size_t)n - 1];
}
