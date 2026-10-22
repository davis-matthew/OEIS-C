/*
 * A380534  a(n) = 1 if the least significant nonzero digit in primorial base
 *          representation of n (A049345) is greater than 1, otherwise 0.
 * https://oeis.org/A380534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 1 if the least significant nonzero digit in primorial
 *            base representation of n is > 1, otherwise 0
 * Range:     u8 n <= 9575, u16 n <= 9575, u32 n <= 9575, u64 n <= 9575,
 *            u128 n <= 9575, big any n
 * Helpers:   primorial_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/primorial_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset guard; not used for OEIS */
    idx_t r = n;
    int k = 0;
    uint64_t digit = 0;
    while (r > 0) {
        uint64_t p = primorial_prime(k);
        digit = r % p;
        if (digit != 0) break;
        r /= p;
        ++k;
    }
    return (seq_t)(digit > 1);
}
