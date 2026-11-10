/*
 * A341943  Fixed points of A341915.
 * https://oeis.org/A341943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(1) = 0, a(2) = 1, a(2k+1) = 8*a(2k-1) + 6, a(2k+2) = 8*a(2k) +
 *            3 for k > 0 * (the empirical recurrence from the OEIS entry,
 *            re-indexed to match the listed terms)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    bool odd = (n & 1) != 0;
    seq_t x = odd ? (seq_t)0 : (seq_t)1;
    idx_t i = odd ? 1 : 2;
    while (i < n) {
        x = x * 8 + (odd ? 6 : 3);
        i += 2;
    }
    return x;
}
