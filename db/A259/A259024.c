/*
 * A259024  a(4*n) = a(n). a(4*n + 2) = 0. a(4*n + 1) = - A259022(n+2). a(4*n
 *          + 3) = - A259022(n-2).
 * https://oeis.org/A259024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) is multiplicative with a(2^e) = (1 + (-1)^e)/2, a(3) = -1,
 *            a(3^e) = 0 if e>1, a(p^e) = 1 if p ≡ 1 (mod 6), a(p^e) = (-1)^e
 *            if p ≡ 5 (mod 6)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mult_a
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mult_a.h"

seq_t a(idx_t n)
{
    /* Directly use the multiplicative definition. */
    return mult_a(n);
}
