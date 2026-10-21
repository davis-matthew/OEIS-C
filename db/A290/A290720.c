/*
 * A290720  a(n) = 2*3^n + 4^n + 3*n.
 * https://oeis.org/A290720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*3^n + 4^n + 3*n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"   /* pow3(idx_t exp) -> seq_t */
#include "../utils/pow4.h"   /* pow4(idx_t exp) -> seq_t */

seq_t a(idx_t n)
{
    /* Compute 2*3^n + 4^n + 3*n, using the provided power helpers. */
    seq_t three_pow   = pow3(n);          /* 3^n   */
    seq_t four_pow    = pow4(n);          /* 4^n   */
    seq_t two_three   = three_pow * 2;    /* 2*3^n */
    seq_t three_n     = 3 * (seq_t)n;     /* 3*n   */
    return two_three + four_pow + three_n;
}
