/*
 * A327187  For any n >= 0: consider the different ways to split the binary
 *          representation of n into two (possibly empty) parts, say with
 *          value x and y; a(n) is the least possible value of x XOR y (where
 *          XOR denotes the bitwise XOR operator).
 * https://oeis.org/A327187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = least possible value of (x XOR y) over all splits of the
 *            binary representation of n into a left part (x) and a right part
 *            (y). For n = 0 return 0
 * Range:     u8 n <= 4507, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* Special case: n == 0 -> by definition a(0) = 0   */
    if (n == 0)
        return (seq_t)0;

    /* Length L of the binary representation of n (most‑significant bit to
       least‑significant bit) */
    unsigned L = floor_log2_uint64(n) + 1;

    seq_t min_val = 0;            /* will be set on first iteration */
    bool first   = true;          /* flag to initialise min_val */

    /* Iterate over all split positions i = 0 … L  (i bits on the left) */
    for (idx_t i = 0; i <= L; ++i) {

        seq_t prefix, suffix;

        /* ------- left part (x) ------- */
        if (i == 0) {
            prefix = (seq_t)0;
        } else {
            /* shift the number right so that only the top i bits remain */
            idx_t shift = L - i;
            /* shifting 64‑bit value by >=64 is undefined, treat as 0 */
            if (shift >= 64)
                prefix = (seq_t)0;
            else
                prefix = (seq_t)(n >> shift);
        }

        /* ------- right part (y) ------- */
        if (i == L) {
            suffix = (seq_t)0;
        } else {
            idx_t bits = L - i;       /* number of bits in the suffix */
            if (bits >= 64) {         /* all bits of n are kept */
                suffix = (seq_t)n;
            } else if (bits == 0) {
                suffix = (seq_t)0;
            } else {
                /* mask to keep the lowest 'bits' bits only */
                uint64_t mask = (1ULL << bits) - 1;
                suffix = (seq_t)(n & mask);
            }
        }

        seq_t val = prefix ^ suffix;

        if (first) {
            min_val = val;
            first = false;
        } else if (val < min_val) {
            min_val = val;
        }
    }

    return min_val;
}
