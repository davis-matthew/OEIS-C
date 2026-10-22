/*
 * A392522  Determinant of the 3 X 3 Hankel matrix of consecutive primes
 *          starting at prime(n).
 * https://oeis.org/A392522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = p(n)*(p(n+2)*p(n+4)-p(n+3)^2) -
 *            p(n+1)*(p(n+1)*p(n+4)-p(n+2)*p(n+3)) +
 *            p(n+2)*(p(n+1)*p(n+3)-p(n+2)^2)
 * Range:     i8 n <= 6, i16 n <= 43, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* fetch five consecutive primes starting at the n‑th prime */
    unsigned long long p0 = (unsigned long long)get_prime(n);
    unsigned long long p1 = (unsigned long long)get_prime(n + 1);
    unsigned long long p2 = (unsigned long long)get_prime(n + 2);
    unsigned long long p3 = (unsigned long long)get_prime(n + 3);
    unsigned long long p4 = (unsigned long long)get_prime(n + 4);

    /* compute the determinant expression using 64‑bit intermediates */
    long long term0 = (long long)p0 * ((long long)p2 * (long long)p4 -
                                      (long long)p3 * (long long)p3);
    long long term1 = - (long long)p1 * ((long long)p1 * (long long)p4 -
                                         (long long)p2 * (long long)p3);
    long long term2 = (long long)p2 * ((long long)p1 * (long long)p3 -
                                      (long long)p2 * (long long)p2);

    return (seq_t)(term0 + term1 + term2);
}
