/*
 * A261470  a(n) = prime(n+3) - prime(n+2) - prime(n+1) + prime(n).
 * https://oeis.org/A261470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = prime(n+3) - prime(n+2) - prime(n+1) + prime(n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* Undefined for n < 1, return 0 for safety */
    seq_t p0 = (seq_t)get_prime(n);          /* prime(n)   */
    seq_t p1 = (seq_t)get_prime(n + 1);      /* prime(n+1) */
    seq_t p2 = (seq_t)get_prime(n + 2);      /* prime(n+2) */
    seq_t p3 = (seq_t)get_prime(n + 3);      /* prime(n+3) */
    return p3 - p2 - p1 + p0;
}
