/*
 * A396239  Numerator of r(n) = (prime(n+2) - 2*prime(n+1) + prime(n)) /
 *          (prime(n+2) - prime(n)).
 * https://oeis.org/A396239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac, easy
 * Formula:   a(n) = numerator of (prime(n+2)-2*prime(n+1)+prime(n)) /
 *            (prime(n+2)-prime(n)) reduced to lowest terms
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Retrieve the necessary primes (1‑indexed). */
    seq_t p1 = (seq_t)get_prime(n);       /* prime(n)   */
    seq_t p2 = (seq_t)get_prime(n + 1);   /* prime(n+1) */
    seq_t p3 = (seq_t)get_prime(n + 2);   /* prime(n+2) */

    /* Compute the numerator of the second difference. */
    seq_t num = p3 - 2 * p2 + p1;
    if (num == 0)
        return 0;                        /* r(n) = 0 */

    /* Denominator of the ratio. */
    seq_t den = p3 - p1;

    /* Reduce the fraction by GCD, preserving the sign of the numerator. */
    seq_t abs_num = num;
    if (abs_num < 0)
        abs_num = -abs_num;

    seq_t g = gcd(abs_num, den);
    return num / g;
}
