/*
 * A400181  a(n) = D_n - A_n, where D_n is the descending permutation and A_n
 *          is the ascending permutation of n copies of each decimal digit
 *          0..9.
 * https://oeis.org/A400181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, new
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1, u128 n <= 3, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A400181:
   a(n) = '9'*n + '7'*n + '5'*n + '3'*n + '1'*(n-1) + '0' +
          '8'*n + '6'*n + '4'*n + '2'*n + '0'*(n-1) + '1'   (decimal)
   Returned value is the integer represented by this 10 n‑digit string.
   Computes using repunit arithmetic and big‑integer power‑of‑ten operations. */
#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but guard anyway */

    /* Pre‑compute reusable values */
    const seq_t pow10_n          = pow10(n);           /* 10^n          */
    const seq_t pow10_n_minus_1  = pow10(n - 1);       /* 10^(n‑1)      */
    const seq_t repunit_n        = (pow10_n - 1) / 9;   /* 111…1 (n digits) */
    const seq_t repunit_n_minus_1= (pow10_n_minus_1 - 1) / 9; /* 111…1 (n‑1 digits) */
    seq_t result = 0;

    /* 9*n */
    result = result * pow10_n + repunit_n * 9;
    /* 7*n */
    result = result * pow10_n + repunit_n * 7;
    /* 5*n */
    result = result * pow10_n + repunit_n * 5;
    /* 3*n */
    result = result * pow10_n + repunit_n * 3;
    /* 1*(n‑1) */
    result = result * pow10_n_minus_1 + repunit_n_minus_1 * 1;
    /* 0 of length 1 */
    result = result * 10;
    /* 8*n */
    result = result * pow10_n + repunit_n * 8;
    /* 6*n */
    result = result * pow10_n + repunit_n * 6;
    /* 4*n */
    result = result * pow10_n + repunit_n * 4;
    /* 2*n */
    result = result * pow10_n + repunit_n * 2;
    /* 0*(n‑1) */
    result = result * pow10_n_minus_1;
    /* final 1 */
    result = result * 10 + 1;

    return result;
}
