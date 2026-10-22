/*
 * A391118  Let B_n = {b_1 < b_2 < ...} be the set of those integers in [n,
 *          n^2] which have a divisor in (n, 2n). a(n) = max(b_(i+1) - b_i).
 * https://oeis.org/A391118
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by marking all numbers in [n, n^2] that have a divisor in (n, 2n)
   and taking the maximum difference between consecutive such numbers. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)3)
        return (seq_t)0;          /* values below the offset are irrelevant */

    /* Work with 64‑bit arithmetic – the bounds are small enough for it. */
    uint64_t n64 = (uint64_t)n;
    uint64_t n2  = n64 * n64;      /* n^2 */
    uint64_t range_len = n2 - n64 + 1;   /* number of integers from n to n^2 inclusive */

    /* Allocate a bitset of that size (8 bits / 1 byte per number = 1 byte/8 = 8 times more efficient). */
    size_t bitlen   = (size_t)((range_len + 63) >> 6); /* number of 64‑bit words */
    uint64_t *bits  = (uint64_t *)calloc(bitlen, sizeof(uint64_t));
    if (!bits)
        return (seq_t)0; /* allocation failure – return a default value (should not occur) */

    /* Mark all multiples of every d in (n, 2n) */
    for (uint64_t d = n64 + 1; d < 2 * n64; ++d) {
        for (uint64_t m = d; m <= n2; m += d) {
            uint64_t idx = m - n64;                 /* offset from n */
            bits[idx >> 6] |= (uint64_t)1 << (idx & 63);
        }
    }

    /* Scan the bitset to compute the maximum gap between consecutive marked numbers. */
    bool has_prev = false;
    uint64_t prev_val = 0;
    uint64_t max_gap  = 0;

    for (uint64_t i = 0; i < range_len; ++i) {
        if (bits[i >> 6] & ((uint64_t)1 << (i & 63))) {
            uint64_t val = n64 + i;
            if (has_prev) {
                uint64_t gap = val - prev_val;
                if (gap > max_gap)
                    max_gap = gap;
            }
            has_prev  = true;
            prev_val  = val;
        }
    }

    free(bits);
    return (seq_t)max_gap;
}
