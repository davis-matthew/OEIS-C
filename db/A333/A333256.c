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
 * Helpers:   decreasing_composition
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
#include "../utils/decreasing_composition.h"

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
