/*
 * A367727  a(n) is the numerator of (R(n) - 1)/(n + 1), where R(n) is the
 *          digit reversal of n.
 * https://oeis.org/A367727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, easy, frac, look
 * Formula:   a(n) = numerator of (rev(n) - 1)/(n + 1)
 * Range:     i8 n <= 101, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9451 (2026-11-01)
 * Helpers:   gcd, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/reverse_digits_idx.h"

seq_t a(idx_t n)
{
    seq_t dn   = (seq_t)n;          /* n as seq_t */
    seq_t revn = (seq_t)reverse_digits_idx(n);     /* reversed integer, cast to seq_t */
    seq_t num  = revn - 1;          /* numerator before reduction */
    seq_t den  = dn + 1;            /* denominator */
    seq_t g    = gcd(num, den);     /* greatest common divisor */
    return num / g;                 /* reduced numerator */
}
