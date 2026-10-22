/*
 * A359475  a(n) = 1 if n is a cubefree nonsquare whose factorization into a
 *          product of primes contains exactly one square, otherwise 0.
 * https://oeis.org/A359475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is cubefree, nonsquare and its prime factorization
 *            contains exactly one squared prime, otherwise 0
 * Range:     u8 n <= 9934, u16 n <= 9934, u32 n <= 9934, u64 n <= 9934,
 *            u128 n <= 9934, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Integer square root for uint64_t: returns floor(sqrt(n)). */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1 << 62;           /* highest power of 4 <= 2^64 */
    while (bit > n)
        bit >>= 2;
    while (bit) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* safety for out of bounds, offset 1 */
        return (seq_t)0;

    /* Reject perfect squares */
    idx_t r = isqrt_u64(n);
    if (r * r == n)
        return (seq_t)0;

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    /* Count exponents == 2 and check cubefree */
    size_t twos = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] > 2)          /* not cubefree */
            return (seq_t)0;
        if (exps[i] == 2)
            twos++;
    }
    return (twos == 1) ? (seq_t)1 : (seq_t)0;
}
