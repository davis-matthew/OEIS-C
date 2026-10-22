/*
 * A338510  a(n) is smallest number in column n >= 0 of the triangle in
 *          A247687.
 * https://oeis.org/A338510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * A014210(n+1)^2 where A014210(m) is the smallest
 *            prime greater than 2^m
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t two_pow = pow2(n);

    /* find the least prime > 2^(n+1) */
    seq_t target = pow2(n + 1);
    seq_t candidate = target + 1;
    while (!is_prime_big(candidate)) {
        candidate = candidate + 1;
    }
    seq_t prime = candidate;

    /* 2^n * prime^2 */
    seq_t result = two_pow * prime * prime;
    return result;
}
