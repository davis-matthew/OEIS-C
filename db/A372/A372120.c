/*
 * A372120  Numbers k such that the k-th composition in standard order is
 *          biquanimous.
 * https://oeis.org/A372120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest nonnegative integer k such that * the
 *            k-th composition in standard order * (graded
 *            reverse‑lexicographic, A066099) is biquanimous
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_biquanimous
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_biquanimous.h"

seq_t a(idx_t n)
{
    /* Branch for the first term (k = 0). */
    if (n == 1) return (seq_t)0;

    idx_t count = 0;
    uint64_t k = 0;

    while (1) {
        if (is_biquanimous(k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        ++k;
    }
}
