/*
 * A290055  Expansion of x*(1 + 4*x + x^2)/((1 - x)^5*(1 + x)^4).
 * https://oeis.org/A290055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (2*n+3+(-1)^n)*(2*n+7+(-1)^n)*(6*n^2+30*n+5-(2*n+5)*(-1)^n)/1536.
 *            Initial terms: a(0)=0, a(1)=1, a(2)=5, a(3)=10, a(4)=26,
 *            a(5)=40, a(6)=80,
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t for exact arithmetic */
    seq_t term1, term2, inner;

    if (n & 1U) {                 /* n is odd: (-1)^n = -1 */
        term1 = 2 * nn + 2;       /* 2*n + 3 - 1  = 2*n + 2   */
        term2 = 2 * nn + 6;       /* 2*n + 7 - 1  = 2*n + 6   */
        inner = 6 * nn * nn + 32 * nn + 10; /* 6*n^2 + 32*n + 10    */
    } else {                      /* n is even: (-1)^n = +1 */
        term1 = 2 * nn + 4;       /* 2*n + 3 + 1  = 2*n + 4   */
        term2 = 2 * nn + 8;       /* 2*n + 7 + 1  = 2*n + 8   */
        inner = 6 * nn * nn + 28 * nn;     /* 6*n^2 + 28*n         */
    }

    seq_t prod = term1 * term2;
    prod = prod * inner;
    prod = prod / 1536;          /* the division is exact for all n */
    return prod;
}
