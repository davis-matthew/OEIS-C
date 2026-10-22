/*
 * A306190  a(n) = p^2 - p - 1 where p = prime(n), the n-th prime.
 * https://oeis.org/A306190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n)^2 - p(n) - 1 where p(n) is the nth prime
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6542, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* get the nth prime (1‑based index) */
    uint32_t p = get_prime(n);
    seq_t pr = (seq_t)p;
    return pr * pr - pr - 1;
}
