/*
 * A259925  a(n) = (n^2 - n - 1)^n.
 * https://oeis.org/A259925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n^2 - n - 1)^n
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 9, i128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"  /* provides: seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * (seq_t)n - (seq_t)n - (seq_t)1;
    return big_pow(base, (seq_t)n);
}
