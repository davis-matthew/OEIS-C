/*
 * A325448  Fibonacci numbers whose decimal expansion contains the digit 1.
 * https://oeis.org/A325448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth Fibonacci number whose decimal expansion contains
 *            the digit 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 25, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> seq_cache;
static seq_t fib_prev = 0;
static seq_t fib_curr = 1;
#else
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static seq_t fib_prev = 0;
static seq_t fib_curr = 1;
#endif

#ifndef __cplusplus
static void seq_ensure_cap_c(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq_cache = tmp;
    seq_cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defensive, although n >= 1 for the sequence */

#ifdef __cplusplus
    if (seq_cache.size() >= n) return seq_cache[n - 1];

    while (seq_cache.size() < n) {
        unsigned char cnt[10];
        digit_counts(fib_curr, cnt);
        if (cnt[1]) {
            if (seq_cache.empty() || fib_curr != seq_cache.back())
                seq_cache.push_back(fib_curr);
        }
        if (seq_cache.size() >= n) break;
        seq_t next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = next;
    }
    return seq_cache[n - 1];
#else
    if (seq_len >= n) return seq_cache[n - 1];

    while (seq_len < n) {
        unsigned char cnt[10];
        digit_counts(fib_curr, cnt);
        if (cnt[1]) {
            if (seq_len == 0 || fib_curr != seq_cache[seq_len - 1]) {
                seq_ensure_cap_c(seq_len + 1);
                seq_cache[seq_len++] = fib_curr;
            }
        }
        if (seq_len >= n) break;
        seq_t next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = next;
    }
    return seq_cache[n - 1];
#endif
}
