/*
 * A296212  a(n) = 1 if sigma(n) + 1 is prime, 0 otherwise.
 * https://oeis.org/A296212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if sigma(n)+1 is prime, 0 otherwise
 * Range:     u8 n <= 107, u16 n <= 9005, u32 n <= 9005, u64 n <= 9005,
 *            u128 n <= 9005, big any n
 * Helpers:   is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t sum = sigma_of(n) + 1;
    return is_prime_big(sum) ? 1 : 0;
}
