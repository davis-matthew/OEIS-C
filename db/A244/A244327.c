/*
 * A244327  a(n) = floor((n*(n+1)/2) / sigma(n)) = floor(A000217(n) /
 *          A000203(n)).
 * https://oeis.org/A244327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( n*(n+1)/2 / sigma(n) ) where sigma(n) is the sum
 *            of positive divisors of n. (A244327)
 * Range:     u8 n <= 227, u16 n <= 9028, u32 n <= 9028, u64 n <= 9028,
 *            u128 n <= 9028, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* For safety against an out‑of‑range call (although the sequence
       is defined for n >= 1). */
    if (n == (idx_t)0) return (seq_t)0;

    /* Compute the triangular number in a type that can hold the
       intermediate without overflow.  unsigned __int128 is a native type
       that is large enough for all n that may be queried (n <= 10000
       during testing). */
    unsigned __int128 tri = (unsigned __int128)n * ((unsigned __int128)n + 1U) / 2;

    /* Retrieve sigma(n) as a seq_t and then widen it for the division.
       sigma_of is guaranteed to fit in 64‑bit for n in the tested range. */
    seq_t sigma_seq = sigma_of(n);
    unsigned __int128 sigma = (unsigned __int128)sigma_seq;

    /* Integer division gives the floor of the ratio. */
    unsigned __int128 div = tri / sigma;

    /* Convert back to seq_t.  The result is guaranteed to fit in seq_t
       for all n that the build tests. */
    return (seq_t)div;
}
