/*
 * A272617  Concatenation of the numbers from n down to 1 with numbers from 1
 *          to n.
 * https://oeis.org/A272617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate of numbers from n down to 1 and from 1 up to
 *            n in base 10
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 100 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/num_digits_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t i = n; i >= 1; --i) {
        seq_t mult = pow10(num_digits_idx(i));
        result = result * mult + (seq_t)i;
    }
    for (idx_t i = 1; i <= n; ++i) {
        seq_t mult = pow10(num_digits_idx(i));
        result = result * mult + (seq_t)i;
    }
    return result;
}
