/*
 * A376279  Numbers k such that k^k is a cube.
 * https://oeis.org/A376279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   integer_cuberoot
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376279: numbers k such that k^k is a cube.
   Equivalently, k is a multiple of 3 or a perfect cube.
   The function returns the nth element (1‑based) of this set. */

#include <stdbool.h>
#include "../utils/integer_cuberoot.h"

static inline bool is_cube(idx_t x)
/* Check whether x is a perfect cube. */
{
    idx_t r = integer_cuberoot(x);
    return r * r * r == x;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;

    while (true) {
        if ((k % 3) == 0 || is_cube(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
