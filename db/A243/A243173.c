/*
 * A243173  Numbers of the form x^2+15y^2.
 * https://oeis.org/A243173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth non‑negative integer of the form x^2 + 15*y^2.
 *            The sequence starts with a(1) = 0, a(2) = 1, a(3) = 4, … as in
 *            OEIS A243173
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static bool is_representable(seq_t m)
{
    seq_t y = 0;
    for (;; ++y) {
        seq_t t = 15 * y * y;
        if (t > m) break;
        seq_t rem = m - t;
        seq_t x = floor_sqrt(rem);
        if (x * x == rem) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; a(1) = 0. */
    idx_t count = 0;
    seq_t v = 0;
    for (;; ++v) {
        if (is_representable(v)) {
            ++count;
            if (count == n) return v;
        }
    }
}
