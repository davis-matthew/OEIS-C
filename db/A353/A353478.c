/*
 * A353478  a(n) = 1 if n is an even semiprime (2*prime), otherwise 0.
 * https://oeis.org/A353478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is 2*prime, otherwise 0
 * Range:     u8 n <= 2309, u16 n <= 9656, u32 n <= 9656, u64 n <= 9656,
 *            u128 n <= 9656, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if ((n & 1) != 0) {
        return 0;
    }
    seq_t p = (seq_t)(n >> 1);          /* n is even, so n/2 is integer */
    return is_prime_small(p) ? 1 : 0;
}
