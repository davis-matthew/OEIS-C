/*
 * A381643  a(n) = 4^n - (n+6)*3^(n-1) + (n+2)*2^(n-1).
 * https://oeis.org/A381643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - (n+6)*3^(n-1) + (n+2)*2^(n-1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 989 (2026-11-01)
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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

seq_t a(idx_t n)
{
    /* For n = 0,1,2 the formula gives 0 for the sequence */
    if (n < 3) {
        return 0;
    }

    /* compute the required powers */
    seq_t pow4_n   = pow4(n);           /* 4^n          */
    seq_t pow3_nm1 = pow3(n - 1);       /* 3^(n-1)      */
    seq_t pow2_nm1 = pow2(n - 1);       /* 2^(n-1)      */

    /* compute the terms */
    seq_t term1 = pow4_n;
    seq_t term2 = pow3_nm1 * ((seq_t)(n + 6));
    seq_t term3 = pow2_nm1 * ((seq_t)(n + 2));

    /* combine them */
    return term1 - term2 + term3;
}
