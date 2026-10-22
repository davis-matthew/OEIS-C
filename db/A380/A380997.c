/*
 * A380997  a(n) is the least number with exactly 2 different decimal digits
 *          that is a multiple of n.
 * https://oeis.org/A380997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) is the least multiple of n that has exactly two distinct
 *            decimal digits
 * Range:     u8 n <= 32, u16 n <= 169, u32 n <= 809, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_two_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_two_digits.h"

/* Entry point: a(n) */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined but guard */
    for (idx_t k = 1;; ++k) {
        seq_t m = (seq_t)k * (seq_t)n;
        if (has_two_digits(m))
            return m;
    }
}
