/*
 * A298252  Even integers n such that n-3 is prime.
 * https://oeis.org/A298252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = prime(n+1) + 3 This gives the even integer n such that
 *            n-3 is prime
 * Range:     u8 n <= 53, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; n = 0 is unused but handled defensively. */
    if (n == 0)
        return (seq_t)0;

    /* Fetch the (n+1)‑th prime using the provided helper and add 3. */
    uint32_t p = get_prime(n + 1);
    return (seq_t)p + (seq_t)3;
}
