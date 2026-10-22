/*
 * A327462  Number of holes in decimal expansion of n-th prime.
 * https://oeis.org/A327462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Number of holes in decimal expansion of the n-th prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* Return 0 for n <= 0 to be safe, even though the sequence is offset 1. */
    if (n < 1) return 0;

    /* Get the n-th prime as a 32-bit unsigned integer. */
    uint32_t prime = get_prime(n);

    /* Count the holes: 0,4,6,9 count 1 each; 8 counts 2. */
    seq_t holes = 0;
    while (prime > 0) {
        uint32_t digit = prime % 10;
        if (digit == 0 || digit == 4 || digit == 6 || digit == 9) {
            holes += 1;
        } else if (digit == 8) {
            holes += 2;
        }
        prime /= 10;
    }
    return holes;
}
