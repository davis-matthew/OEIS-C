/*
 * A254464  a(n) = 21*2^n + 10*4^n + 15*3^n + 3*6^n + 6*5^n + 7^n + 28.
 * https://oeis.org/A254464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*2^n + 10*4^n + 15*3^n + 3*6^n + 6*5^n + 7^n + 28
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 978 (2026-11-01)
 * Helpers:   ipow_idx, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_idx.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t p2 = pow2(n);          /* 2^n */
    seq_t p3 = pow3(n);          /* 3^n */
    seq_t p4 = pow4(n);          /* 4^n */
    seq_t p5 = power5(n);        /* 5^n */
    seq_t p7 = ipow_idx((seq_t)7, n);  /* 7^n */
    seq_t p6 = p2 * p3;          /* 6^n = 2^n * 3^n */

    return 21 * p2 + 10 * p4 + 15 * p3 + 3 * p6 + 6 * p5 + p7 + 28;
}
