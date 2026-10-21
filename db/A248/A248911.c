/*
 * A248911  a(n) = floor( prime(n) - (n+1)*log(n) ).
 * https://oeis.org/A248911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( prime(n) - (n+1)*log(n) ) for n ≥ 1
 * Range:     u8 n <= 295, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the nth‑prime routine from ../utils/get_prime.h
   and the standard C library for logarithm and floor.  All arithmetic on
   the target integer type seq_t is performed only after the real
   computation has been reduced to a native integer.  The result always
   fits into a signed 64‑bit integer for the n values tested here, so the
   conversion is safe. */

#include <math.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n = 0, return 0 */

    /* obtain the n‑th prime as a native integer */
    uint32_t p = get_prime(n);

    /* compute (n+1)*log(n) using long double for good precision */
    long double ln = log((long double)n);
    long double term = ((long double)(n + 1)) * ln;

    /* difference and floor */
    long double diff = (long double)p - term;
    long long  result = (long long)floor(diff);

    /* cast back to the generic seq_t type */
    return (seq_t)result;
}
