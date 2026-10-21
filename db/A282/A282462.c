/*
 * A282462  Integers but with the primes cubed.
 * https://oeis.org/A282462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (is_prime(n) ? n^3 : n)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1626, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return is_prime(n) ? x * x * x : x;
}
