/*
 * A245804  a(n) = 2*3^n - 3*2^n.
 * https://oeis.org/A245804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*3^n - 3*2^n The function computes the closed‑form
 *            directly by calling the provided pow2 and pow3 utilities
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 18, i64 n <= 39, i128 n <= 79,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t p3 = pow3(n);          /* 3^n */
    seq_t p2 = pow2(n);          /* 2^n */
    return (seq_t)2 * p3 - (seq_t)3 * p2;
}
