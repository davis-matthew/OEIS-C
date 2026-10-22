/*
 * A342567  a(n) = (prime(n)^2 - prime(n-1)*prime(n+1))/2, n >= 3.
 * https://oeis.org/A342567
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (prime(n)^2 - prime(n-1)*prime(n+1))/2, n ≥ 3
 * Range:     i8 n <= 9, i16 n <= 428, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;       /* undefined for n < 3, return 0 */

    seq_t p   = (seq_t)get_prime(n);      /* prime(n)   */
    seq_t pm1 = (seq_t)get_prime(n - 1);  /* prime(n-1) */
    seq_t pp1 = (seq_t)get_prime(n + 1);  /* prime(n+1) */

    seq_t diff = p * p - pm1 * pp1;       /* numerator, always even */
    return diff / (seq_t)2;
}
