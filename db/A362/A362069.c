/*
 * A362069  Numbers k such that k+digitsum(k^2) is a square.
 * https://oeis.org/A362069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   sequences of numbers k such that k + digit_sum(k^2) is a square
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   digit_sum, ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds the sequence incrementally and caches discovered terms. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    /* static storage for discovered terms */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;   /* how many terms stored */
    static size_t cache_cap  = 0;   /* capacity of the array */
    static idx_t next_k      = 0;   /* next integer to test */

    if (n == 0) {
        /* undefined for n < 1; return 0 to avoid UB */
        return (seq_t)0;
    }

    /* Generate terms until we have at least n of them */
    while (cache_size < n) {
        seq_t k_val = (seq_t)next_k;
        seq_t k_sq  = k_val * k_val;
        seq_t dsum  = digit_sum(k_sq);
        seq_t sum   = k_val + dsum;
        seq_t sq    = floor_sqrt(sum);

        if (sq * sq == sum) {
            /* grow the cache if needed */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = k_val;
        }
        next_k++;
    }

    return cache[n - 1];
}
