/*
 * A287922  a(n) = prime(1)^2 + prime(n)^2.
 * https://oeis.org/A287922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(1)^2 + prime(n)^2 = 4 + prime(n)^2
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    uint32_t p32 = get_prime(n);   /* n is 1‑based, get_prime(1)=2 */
    seq_t p = (seq_t)p32;
    return (seq_t)4 + p * p;
}
