/*
 * A274698  a(n)=prime(n)-(2*last digit of prime(n)).
 * https://oeis.org/A274698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, less
 * Formula:   not stated
 * Range:     i8 n <= 31, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = prime(n) - 2*(prime(n) % 10) */
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t p = (seq_t)get_prime(n);
    seq_t last = p % (seq_t)10;
    return p - (2 * last);
}
