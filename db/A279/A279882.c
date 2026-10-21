/*
 * A279882  a(n) = 2^(prime(n) + 1) - 1.
 * https://oeis.org/A279882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(prime(n)+1) - 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"   /* returns uint32_t get_prime(idx_t k) */
#include "../utils/pow2.h"       /* returns seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; n should be ≥ 1.  For safety, handle n == 0. */
    if (n == 0) return 0;

    /* Get the n-th prime.  get_prime(1) == 2. */
    uint32_t p = get_prime(n);

    /* Compute 2^(p+1) and subtract 1. */
    seq_t val = pow2((idx_t)p + 1);
    return val - (seq_t)1;
}
