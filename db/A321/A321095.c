/*
 * A321095  Continued fraction expansion of the constant z that satisfies:
 *          CF(5*z, n) = CF(z, n) + 36, for n >= 0, where CF(z, n) denotes the
 *          n-th partial denominator in the continued fraction expansion of z.
 * https://oeis.org/A321095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   not stated
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Validated: partial, pass, 201 terms through n = 200 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A321095
 * Formula: 
 *   a(0) = 8
 *   a(3*n) = 7 for n >= 1
 *   a(3*n+1) = f(n) where
 *        f(0) = 1
 *        f(2) = 4
 *        f(3*m+1) = 5 - f(m)          (for m >= 0)
 *   a(3*n+2) = 5 - a(3*n+1)           (for n >= 0)
 *   (This implements the OEIS given rules.)
 */
#include <stdint.h>

static seq_t f_value(idx_t n) {
    /* Recursive helper for a(3*n+1) values 1 or 4. */
    if (n % 3 == 0) {
        /* n = 0, 3, 6, ...  -> a(9*k+1) = 1 */
        return (seq_t)1;
    }
    if (n % 3 == 2) {
        /* n = 2, 5, 8, ...  -> a(9*k+7) = 4 */
        return (seq_t)4;
    }
    /* n % 3 == 1 :  n = 3*m + 1  -> a(9*m+4) = 5 - a(3*m+1) */
    return (seq_t)(5 - f_value((n - 1) / 3));
}

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)8;
    }
    switch (n % 3) {
        case 0:
            /* Indices 3,6,9,... are 7 (except 0 handled above) */
            return (seq_t)7;
        case 1:
            /* Indices 1,4,7,... use f_value */
            return f_value((n - 1) / 3);
        case 2:
            /* a(3*k+2) = 5 - a(3*k+1) */
            return (seq_t)(5 - f_value((n - 2) / 3));
    }
    /* Unreachable, but keep a return for strict compilers. */
    return (seq_t)0;
}
