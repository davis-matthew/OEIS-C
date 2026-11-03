/*
 * A333256  Numbers k such that the k-th composition in standard order is
 *          strictly decreasing.
 * https://oeis.org/A333256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A333256: numbers k such that the k‑th composition in standard
   order is strictly decreasing.  The sequence is computed by iterating
   over all k ≥ 0 and checking that the gaps between successive 1‑bits
   in the binary representation of k increase strictly.  The n‑th
   qualifying k (with offset 1) is returned. */

#include <stdbool.h>
#include <stdint.h>

/* Helper: returns true iff the composition associated with k is
   strictly decreasing.  k is interpreted as an unsigned 64‑bit
   integer. */
static bool decreasing_composition(idx_t k)
{
    if (k == 0)            /* empty composition (k = 0) is acceptable */
        return true;

    idx_t temp = k;
    int prev_idx = -1;     /* index of the previous set bit */
    int prev_gap = 0;      /* previous gap size (D_{j-1}) */
    bool first = true;     /* flag for the first set bit */

    while (temp) {
        /* locate the lowest set bit (zero‑based index) */
        int idx = __builtin_ctzll(temp);   /* CTZ works on unsigned long long */
        if (first) {
            prev_gap = idx + 1;            /* D1 = idx + 1 */
            first = false;
        } else {
            int cur_gap = idx - prev_idx;   /* D_j = idx - previous_index */
            if (cur_gap <= prev_gap)       /* need strictly increasing gaps */
                return false;
            prev_gap = cur_gap;
        }
        prev_idx = idx;
        temp &= temp - 1;                 /* clear lowest set bit */
    }
    return true;
}

/* Main sequence function.  a(1) = 0 according to the OEIS offset. */
seq_t a(idx_t n)
{
    idx_t k = 0;
    idx_t count = 0;

    while (true) {
        if (decreasing_composition(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
