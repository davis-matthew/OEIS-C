/*
 * A333381  Number of maximal anti-runs of the n-th composition in standard
 *          order.
 * https://oeis.org/A333381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A124762(n) + 1. A124762(n) is the number of adjacent
 *            equal pairs in the nth composition in standard order (A066099).
 *            This function computes a(n) for any n ≥ 0
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(0) is defined as 0. */
    if (n == 0) return (seq_t)0;

    /* Find m such that 2^m - 1 >= n.
       Equivalently, 2^m >= n + 1.  Compute by repeated doubling. */
    idx_t pow = 1;      /* pow = 2^m */
    idx_t m   = 0;
    while (pow < n + 1) {
        pow <<= 1;
        m++;
    }

    /* Number of compositions before row m : 2^(m-1) - 1 */
    idx_t prev_pow = pow >> 1;            /* 2^(m-1) */
    idx_t k = n - prev_pow;               /* zero‑based index within row m */

    /* Build the composition of m corresponding to k.
       Iterate over the (m-1) separator bits of k. */
    idx_t parts_before = 0;                /* 1 if a part has already been emitted */
    seq_t prev_part_len = 0;
    idx_t curr_len = 1;                    /* current part length (in idx_t, but fits) */
    seq_t equal_pairs = 0;

    for (idx_t i = 0; i < m - 1; ++i) {
        /* Bit position from left (most significant) to right. */
        idx_t bit_pos = (m - 2) - i;
        idx_t bit = (k >> bit_pos) & 1;

        if (bit) { /* separator after this element */
            if (parts_before) {
                if (curr_len == prev_part_len) equal_pairs++;
            }
            parts_before = 1;
            prev_part_len = curr_len;
            curr_len = 1;
        } else {
            curr_len++;
        }
    }

    /* Compare the final part with the previous one. */
    if (parts_before && curr_len == prev_part_len) {
        equal_pairs++;
    }

    /* Result is the number of equal adjacent pairs plus one. */
    return (seq_t)(equal_pairs + (seq_t)1);
}
