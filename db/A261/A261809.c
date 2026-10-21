/*
 * A261809  a(n) = n! - prime(n).
 * https://oeis.org/A261809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! - prime(n)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   factorial, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t f = factorial(n);
    seq_t p = (seq_t) get_prime(n);
    return f - p;
}
