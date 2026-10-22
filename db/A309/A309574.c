/*
 * A309574  n-th prime minus its ternary (base 3) reversal.
 * https://oeis.org/A309574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) = get_prime(n) - ternary_reverse(get_prime(n))
 * Range:     i8 n <= 31, i16 n <= 2230, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime, ternary_reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/ternary_reverse.h"

seq_t a(idx_t n)
{
    seq_t p = get_prime(n);
    seq_t rev = ternary_reverse(p);
    return p - rev;
}
