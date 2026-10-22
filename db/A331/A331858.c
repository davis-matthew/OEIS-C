/*
 * A331858  a(n) = (2^p-1)*(2^(p-1))*((2^p-1)^2-2), where p is the n-th prime.
 * https://oeis.org/A331858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^p-1)*(2^(p-1))*((2^p-1)^2-2), where p = prime(n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Obtain the n‑th prime (p ≥ 2). */
    uint32_t p_u32 = get_prime(n);
    idx_t p = (idx_t)p_u32;

    /* Compute 2^p and 2^(p-1). */
    seq_t two_p = pow2(p);              /* 2^p   */
    seq_t two_p_minus1 = two_p - 1;     /* 2^p-1 */

    /* Part 1: (2^p-1)*2^(p-1). */
    seq_t part1 = two_p_minus1 * pow2(p - 1);

    /* Part 2: (2^p-1)^2 - 2. */
    seq_t part2 = two_p_minus1 * two_p_minus1 - 2;

    /* Final result. */
    return part1 * part2;
}
