/*
 * A243895  a(n) = prime(n^2-1).
 * https://oeis.org/A243895
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n^2-1)
 * Range:     u8 n <= 7, u16 n <= 80, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Undefined for n < 2, return 0 in that case. */
    if (n < 2) return 0;
    idx_t k = n * n - 1;
    return (seq_t)get_prime(k);
}
