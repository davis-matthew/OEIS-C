/*
 * A397466  a(n) = prime(floor(n*sqrt(2))).
 * https://oeis.org/A397466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(floor(n*sqrt(2)))
 * Range:     u8 n <= 5, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/get_prime.h"

/* Computes the n-th term of OEIS sequence A397466.
 * a(n) = prime( floor( n * sqrt(2) ) ),  with offset 1. */
seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 by the OEIS offset. */
    seq_t nseq = (seq_t)n;
    /* Compute floor(n * sqrt(2)) as floor( sqrt(2*n^2) ). */
    seq_t m = (seq_t)2 * nseq * nseq;   /* 2*n^2 */
    idx_t idx = (idx_t)floor_sqrt(m);   /* floor(sqrt(m)) = floor(n*sqrt(2)) */
    /* Return the idx-th prime (1-indexed). */
    return (seq_t)get_prime(idx);
}
