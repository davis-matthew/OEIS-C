/*
 * A281509  Trajectory of 1999291987030606810 (the largest presently known
 *          "most delayed palindrome") under the "Reverse and Add!" operation.
 * https://oeis.org/A281509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(0) = 1999291987030606810; a(n+1) = a(n) + rev(a(n)) (decimal
 *            reversal)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 4, u128 n <= 56, big any n
 * Validated: standard, pass, 29 terms through n = 249 (2026-11-01)
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/reverse_digits.h"
#include <stddef.h>

seq_t a(idx_t n) {
    seq_t val = (seq_t)1999291987030606810ULL;
    for (idx_t i = 0; i < n; ++i) {
        seq_t rev = reverse_digits(val);
        val = val + rev;
    }
    return val;
}
