/*
 * A360240  Weakly decreasing triples of positive integers sorted
 *          lexicographically and concatenated.
 * https://oeis.org/A360240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A331195(n-1)+1. The sequence is the digits (1‑based) of
 *            the concatenation of all weakly decreasing triples of positive
 *            integers sorted lexicographically. This implementation generates
 *            the triples in that order and extracts the requested digit
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   digit_from_num
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_from_num.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, return 0 */

    idx_t pos = n - 1;              /* zero‑based index */
    seq_t digit = 0;

    for (idx_t x = 1; ; ++x) {
        for (idx_t y = 1; y <= x; ++y) {
            for (idx_t z = 1; z <= y; ++z) {
                if (digit_from_num(x, &pos, &digit)) return digit;
                if (digit_from_num(y, &pos, &digit)) return digit;
                if (digit_from_num(z, &pos, &digit)) return digit;
            }
        }
    }
}
