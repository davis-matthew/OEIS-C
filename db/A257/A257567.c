/*
 * A257567  a(n) is the largest exponent k such that 3^k divides (prime(n)^2 +
 *          2).
 * https://oeis.org/A257567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6774, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = exponent of 3 in prime(n)^2 + 2 */
#include "../utils/get_prime.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* get the n-th prime (1‑based) */
    uint32_t p = get_prime(n);

    /* compute p^2 + 2 in the register type seq_t */
    seq_t val = (seq_t)p * (seq_t)p + 2;

    /* count the power of 3 dividing val */
    seq_t k = 0;
    while (val % 3 == 0) {
        k += 1;
        val /= 3;
    }
    return k;
}
