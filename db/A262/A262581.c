/*
 * A262581  Concatenation of the numbers from 1 to n but omitting 11.
 * https://oeis.org/A262581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of integers 1..m, where m = n + (n >= 11 ?
 *            1 : 0), omitting the integer 11
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-01)
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
    idx_t m = n;
    if (n >= 11) m = n + 1;
    seq_t result = 0;
    for (idx_t i = 1; i <= m; ++i) {
        if (i == 11) continue;
        result = result * pow10(num_digits_idx(i)) + (seq_t)i;
    }
    return result;
}
