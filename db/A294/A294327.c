/*
 * A294327  a(n) = ((9*n + 8)*10^n - 8)/9.
 * https://oeis.org/A294327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((9*n + 8)*10^n - 8)/9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 807 (2026-11-01)
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

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;                 /* cast index to seq_t */
    seq_t nine_n_plus_8 = tn * 9 + 8;    /* 9*n + 8            */
    seq_t ten_n = pow10(n);              /* 10^n                */
    seq_t numerator = nine_n_plus_8 * ten_n - 8; /* (9*n+8)*10^n - 8 */
    seq_t result = numerator / 9;        /* divide by 9         */
    return result;
}
