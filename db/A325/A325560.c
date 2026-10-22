/*
 * A325560  a(n) is the number of divisors d of n such that A048720(d,k) = n
 *          for some k.
 * https://oeis.org/A325560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of divisors d of n such that the
 *            binary‐polynomial of d divides the binary‐polynomial of n over
 *            GF(2). The function enumerates all divisors of n and checks the
 *            divisibility by a standard GF(2) polynomial division
 * Range:     u8 n <= 9250, u16 n <= 9250, u32 n <= 9250, u64 n <= 9250,
 *            u128 n <= 9250, big any n
 * Helpers:   poly_divides_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/poly_divides_uint64.h"

seq_t a(idx_t n)
{
    /* convert to native unsigned type */
    uint64_t N = (uint64_t)n;
    uint64_t count = 0;

    /* enumerate all divisors of N */
    for (uint64_t d = 1; d * d <= N; ++d) {
        if (N % d == 0) {
            if (poly_divides_uint64(N, d)) ++count;
            uint64_t d2 = N / d;
            if (d2 != d && poly_divides_uint64(N, d2)) ++count;
        }
    }

    return (seq_t)count;
}
