/*
 * A272066  a(n) = (10^n-1)^3.
 * https://oeis.org/A272066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n-1)^3
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 12 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"

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
    seq_t t = pow10(n);   /* t = 10^n */
    t -= 1;               /* t = 10^n - 1 */
    return ipow_idx(t, 3); /* return (10^n - 1)^3 */
}
