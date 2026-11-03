/*
 * A343275  a(n) = |2*n - 10^length(n)|.
 * https://oeis.org/A343275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |2*n - 10^digits(n)|
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 86 terms through n = 9943 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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

seq_t a(idx_t n)
{
    // n is at least 1
    int dn = num_digits_idx(n);          // number of decimal digits of n
    seq_t m   = pow10(dn);       // 10^digits(n)
    seq_t two_n = (seq_t)n * 2;  // 2*n
    return two_n > m ? two_n - m : m - two_n; // absolute difference
}
