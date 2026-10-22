/*
 * A376645  The maximum exponent in the factorization of n into powers of
 *          Gaussian primes.
 * https://oeis.org/A376645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   max_prime_exponent_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A376645: maximum exponent in the factorization of n into
 * powers of Gaussian primes.
 * Formula: a(n) = max( 2*v2(n) , max_exponent_in_prime_factors( odd_part(n) ) )
 * where v2(n) is the exponent of 2 in n and odd_part(n) = n / 2^v2(n).
 * The second term is simply the largest exponent appearing in the
 * ordinary integer factorization of the odd part.
 *
 * The implementation uses trial division on the odd part and
 * computes 2*v2(n) by repeated right-shifts.  It works for any
 * unsigned integer type seq_t for the return value and idx_t for
 * the input index, with no external dependencies.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/max_prime_exponent_odd.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                  /* not used by OEIS but keeps the function total */
        return (seq_t)0;

    /* v2(n) and odd part */
    idx_t v2 = 0;
    idx_t temp = n;
    while ((temp & 1ULL) == 0) {
        ++v2;
        temp >>= 1;
    }
    idx_t odd_part = temp;

    /* 2*v2(n) */
    seq_t two_v2 = (seq_t)2 * (seq_t)v2;

    /* max exponent in the odd part */
    seq_t odd_exp = max_prime_exponent_odd(odd_part);

    /* Return the maximum of the two terms */
    return (two_v2 > odd_exp) ? two_v2 : odd_exp;
}
