/*
 * A382161  "Repunit" Kaprekar numbers.
 * https://oeis.org/A382161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^(9*n-8)-1)/9
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 24 terms through n = 93 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

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
    /* compute exponent: e = 9*n - 8 */
    idx_t e = 9*n - 8ull;
    seq_t p = pow10(e);
    /* (10^e - 1) / 9 */
    p = p - (seq_t)1;
    p = p / (seq_t)9;
    return p;
}
