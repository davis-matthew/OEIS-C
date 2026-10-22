/*
 * A392285  a(n) = omega(n) * (-1)^(bigomega(n) + 1).
 * https://oeis.org/A392285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = omega(n) * (-1)^(bigomega(n)+1)
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   bigomega, count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_distinct_primes.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* Number of distinct prime factors of n */
    seq_t omega = (seq_t)count_distinct_primes((uint64_t)n);

    /* Total number of prime factors of n (with multiplicity) */
    seq_t big = bigomega(n);

    /* Sign: 1 if bigodd, -1 if big even */
    seq_t sign = ((big & 1) == 0) ? -1 : 1;

    return omega * sign;
}
