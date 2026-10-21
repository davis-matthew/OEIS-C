/*
 * A295297  a(n) = (A000120(n) + A000203(n)) mod 2.
 * https://oeis.org/A295297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (popcount(n) + sigma(n)) mod 2 where sigma(n) is the sum
 *            of the positive divisors of n (A000203). The parity of sigma(n)
 *            is 1 iff the odd part of n is a perfect square
 * Range:     u8 n <= 64, u16 n <= 9204, u32 n <= 9204, u64 n <= 9204,
 *            u128 n <= 9204, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* external linkage as required */
seq_t a(idx_t n)
{
    /* parity of binary weight of n (popcount mod 2) */
    uint8_t pop_parity = 0;
    idx_t temp = n;
    while (temp) {
        pop_parity ^= (uint8_t)(temp & 1);
        temp >>= 1;
    }

    /* remove all factors of 2 to obtain the odd part of n */
    idx_t odd = n;
    while ((odd & 1ULL) == 0ULL)
        odd >>= 1;

    /* check if odd part is a perfect square */
    seq_t odd_seq = (seq_t)odd;
    seq_t s = floor_sqrt(odd_seq);            /* floor(sqrt(odd)) */
    seq_t s2 = s * s;
    uint8_t sigma_parity = (s2 == odd_seq) ? 1 : 0;

    /* final result: XOR of the two parities */
    return (seq_t)(pop_parity ^ sigma_parity);
}
