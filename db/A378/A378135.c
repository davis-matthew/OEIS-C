/*
 * A378135  a(n) = p(n*p(n)), where p(x) = least prime > x.
 * https://oeis.org/A378135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n * p(n)), where p(x) = least prime > x
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* least prime greater than n */
    uint64_t p1 = next_prime((uint64_t)n);

    /* product n * p1 (fits in uint64_t for the intended range) */
    uint64_t prod = (uint64_t)n * p1;

    /* least prime greater than the product */
    uint64_t p2 = next_prime(prod);

    return (seq_t)p2;
}
