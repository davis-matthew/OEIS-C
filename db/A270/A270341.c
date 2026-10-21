/*
 * A270341  Numbers that equal the sum of the squares of the digits of their
 *          own square in base 43.
 * https://oeis.org/A270341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base
 * Formula:   a(n) = the n-th integer that equals the sum of the squares of
 *            the digits of its own square when the digits are taken in base
 *            43
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   digit_square_sum_of_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_square_sum_of_square.h"

/* a(idx_t n) – 1‑based indexing (a(1)=0, a(2)=1, …)
   Works for all provided seq_t and idx_t types. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t last = 0;
    for (idx_t cand = 0; found < n; ++cand) {
        seq_t val = (seq_t)cand;
        if (digit_square_sum_of_square(val) == val) {
            ++found;
            last = val;
        }
    }
    return last;
}
