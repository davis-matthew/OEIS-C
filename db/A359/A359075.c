/*
 * A359075  Numbers that do not have two divisors with an equal sum of digits.
 * https://oeis.org/A359075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-03)
 * Helpers:   digit_sum, ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth positive integer whose divisors all have distinct digit sums. */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

#define MAX_DIVISORS 4096  /* enough for divisor counts of all practical candidates */

/* Return true if `x` has two divisors with the same digit sum. */
static bool has_dup_digit_sum_divisors(seq_t x)
{
    seq_t sums[MAX_DIVISORS];
    size_t sumcnt = 0;

    seq_t sroot = floor_sqrt(x);
    for (seq_t d = 1; d <= sroot; ++d) {
        if (x % d == 0) {
            seq_t sd = digit_sum(d);
            for (size_t i = 0; i < sumcnt; ++i) {
                if (sums[i] == sd) return true;
            }
            sums[sumcnt++] = sd;

            seq_t q = x / d;
            if (q != d) {
                seq_t sq = digit_sum(q);
                for (size_t i = 0; i < sumcnt; ++i) {
                    if (sums[i] == sq) return true;
                }
                sums[sumcnt++] = sq;
            }
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for offset < 1 */

    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static seq_t next_candidate = 1;

    /* Expand cache if needed */
    if ((size_t)n > cache_size) {
        size_t needed = (size_t)n;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap : 16;
            while (new_cap < needed) new_cap *= 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate terms until we have n of them */
        while (cache_size < needed) {
            /* find next valid integer */
            while (has_dup_digit_sum_divisors(next_candidate))
                ++next_candidate;

            cache[cache_size++] = next_candidate;
            ++next_candidate;
        }
    }

    return cache[n - 1];   /* offset 1: a(1) is first element */
}
