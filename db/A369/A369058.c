/*
 * A369058  a(n) = 1 if n is a product of three (not necessarily distinct) odd
 *          primes, otherwise 0.
 * https://oeis.org/A369058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is the product of exactly three odd primes,
 *            otherwise 0 (i.e., a(n) = A000035(n) * A101605(n))
 * Range:     u8 n <= 9972, u16 n <= 9972, u32 n <= 9972, u64 n <= 9972,
 *            u128 n <= 9972, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range offset is irrelevant */
    if ((n & 1ULL) == 0) return 0; /* even numbers cannot be products of odd primes */

    seq_t omega = bigomega(n);       /* total prime factors (with multiplicity) */
    return (omega == (seq_t)3) ? 1 : 0;
}
