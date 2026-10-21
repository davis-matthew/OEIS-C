/*
 * A245034  a(n) = prime(n)^2 - 4*prime(n).
 * https://oeis.org/A245034
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^2 - 4*prime(n)
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 6543, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n < 3) {
        return 0;
    }
    uint32_t p = get_prime(n);
    seq_t sp = (seq_t)p;
    return sp * sp - sp * (seq_t)4;
}
