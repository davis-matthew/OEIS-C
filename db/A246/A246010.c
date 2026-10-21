/*
 * A246010  a(n) = floor(5*prime(n)^2 / 4).
 * https://oeis.org/A246010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(5*prime(n)^2 / 4)
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 1044, u64 n <= 1044,
 *            u128 n <= 1044, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    seq_t p = (seq_t)get_prime(n);
    seq_t v = p * p;
    v = v * 5;
    return v / 4;
}
