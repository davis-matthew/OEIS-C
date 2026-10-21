/*
 * A258322  Expansion of phi(-q) * phi(-q^9) in powers of q where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A258322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   b_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A258322: Expansion of phi(-q) * phi(-q^9).
 * Formula:
 *   a(0) = 1
 *   For n > 0:
 *        if n % 3 == 2    -> a(n) = 0
 *        else             -> a(n) = -2 * b(n)
 *   where b(n) is a multiplicative function defined by
 *        b(1) = 1
 *        b(2^e) = -1           (e > 0)
 *        b(3^e) = 1 + (-1)^e   (e > 0)
 *        b(p^e) = (1 + (-1)^e)/2  if p ≡ 3 (mod 4)
 *        b(p^e) = e + 1          if p ≡ 1 (mod 4)   (p prime)
 *   For composite n, b(n) = Π_p b(p^e) over its prime factors.
 */

#include <stdint.h>
#include "../utils/b_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    if ((n % 3) == 2) return 0;

    seq_t b = b_of(n);
    return - ((seq_t)2 * b);
}
