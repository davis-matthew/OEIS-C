/*
 * A254958  Zeroless numbers n with digits d_1, d_2, ... d_k such that d_1^2 +
 *          ... + d_k^2 is a square.
 * https://oeis.org/A254958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Sequence A254958: zeroless numbers n such that the sum of the squares of
   its decimal digits is a perfect square.  The function searches for the
   nth such number. */

#define MAX_CACHE 30000      /* enough for n up to about 10 000 */

static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;
static uint64_t next_candidate = 1;

static inline bool has_zero_digit(uint64_t x)
{
    while (x > 0) {
        if (x % 10 == 0)
            return true;
        x /= 10;
    }
    return false;
}

/* Return a(n) for any n ≥ 1, in any order of calls. */
seq_t a(idx_t n)
{
    /* Fill cache up to n. */
    while (cache_len < (size_t)n) {
        uint64_t x = next_candidate++;
        if (has_zero_digit(x))
            continue;

        /* compute sum of square of digits */
        uint64_t t = x;
        uint64_t sum_sq = 0;
        while (t > 0) {
            uint8_t d = (uint8_t)(t % 10);
            sum_sq += (uint64_t)d * (uint64_t)d;
            t /= 10;
        }

        /* Quick bound: if sum_sq does not fit in seq_t, skip */
        if (sum_sq > (uint64_t)(seq_t)-1)
            continue;

        seq_t sum = (seq_t)sum_sq;
        seq_t root = floor_sqrt(sum);
        if (root * root == sum) {
            if (cache_len >= MAX_CACHE) {
                /* Safety: should not happen for n ≤ 10 000 */
                exit(EXIT_FAILURE);
            }
            cache[cache_len++] = (seq_t)x;
        }
    }
    return cache[(size_t)n - 1];
}
