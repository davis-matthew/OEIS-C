/*
 * A253936  a(n) = prime(n + (prime(n) mod 10)).
 * https://oeis.org/A253936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000040(n + (A000040(n) mod 10))
 * Range:     u8 n <= 45, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* prime(n) */
    uint32_t p = get_prime(n);

    /* last digit of p */
    uint32_t r = p % 10U;

    /* target index */
    idx_t target = n + r;

    /* prime(target) */
    uint32_t result = get_prime(target);

    return (seq_t)result;
}
