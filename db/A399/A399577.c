/*
 * A399577  Happy numbers whose trajectories under iteration of the sum of
 *          squares of digits map to 10 before going to the value of 1.
 * https://oeis.org/A399577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 4389, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A399577: happy numbers that hit 10 before 1
   by brute‑force search of consecutive integers. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/digit_counts.h"

/* Sum of squares of decimal digits for an arbitrary seq_t value. */
static seq_t digit_square_sum_seq(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    seq_t sum = 0;
    for (int d = 0; d < 10; ++d) {
        if (cnt[d]) {
            seq_t mult = (seq_t)(d * d);
            sum += mult * (seq_t)cnt[d];
        }
    }
    return sum;
}

/* Returns true if n satisfies the sequence condition. */
static bool is_good(idx_t n)
{
    seq_t cur = (seq_t)n;
    bool seen10 = (cur == (seq_t)10);

    /* Reduce very large values to < 2000 to keep cycle detection small. */
    if (cur > (seq_t)2000) {
        while (cur > (seq_t)2000) {
            cur = digit_square_sum_seq(cur);
            if (cur == (seq_t)10) seen10 = true;
        }
    }

    /* Small cycle detection for values <= 2000. */
    static uint16_t visited[2001] = {0};
    static uint16_t stamp = 1;
    ++stamp;
    if (stamp == 0) {                      /* stamp wrapped */
        memset(visited, 0, sizeof(visited));
        stamp = 1;
    }

    if (cur <= (seq_t)2000)
        visited[(int)((uint64_t)cur)] = stamp;

    while (true) {
        if (cur == (seq_t)1) {
            return seen10;                /* reached 1, 10 must have appeared */
        }
        cur = digit_square_sum_seq(cur);
        if (cur == (seq_t)10) seen10 = true;
        if (cur > (seq_t)2000) continue;   /* should not happen after reduction */
        if (visited[(int)((uint64_t)cur)] == stamp)
            return false;                 /* cycle before reaching 1 */
        visited[(int)((uint64_t)cur)] = stamp;
    }
}

/* Cache of found sequence terms for successive calls. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t next_candidate = 1;

seq_t a(idx_t n)
{
    while (cache_len < n) {
        if (is_good(next_candidate)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
