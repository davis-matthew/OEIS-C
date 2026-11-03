/*
 * A340679  If n is a power of prime then a(n) = 1, otherwise a(n) = product
 *          of the distinct prime factors of n.
 * https://oeis.org/A340679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   if n is a power of a prime then a(n)=1, otherwise a(n)=product
 *            of the distinct prime factors of n
 * Range:     u8 n <= 257, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 108 terms through n = 9749 (2026-11-01)
 * Helpers:   omega, radical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/omega.h"
#include "../utils/radical.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n <= 1)
        return 1;

    /* Count how many distinct primes divide n */
    int d = omega((uint64_t)n);

    /* If n has zero or one distinct prime factor we return 1
       (n = 1 or n is a prime power).  Otherwise return the
       product of distinct prime factors, i.e. rad(n). */
    if (d <= 1)
        return 1;
    else
        return radical(n);
}
