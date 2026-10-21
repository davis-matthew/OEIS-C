/*
 * A257230  Floor(sqrt(q)-(q-p)), where p and q are consecutive primes.
 * https://oeis.org/A257230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = floor(sqrt(q)) - (q - p), where p = prime(n), q =
 *            prime(n+1)
 * Range:     i8 n <= 1948, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* Integer square‑root for 32‑bit unsigned values */
static uint32_t isqrt_u32(uint32_t x)
{
    uint32_t low = 0, high = x, mid, res = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        uint64_t sq = (uint64_t)mid * (uint64_t)mid;
        if (sq <= (uint64_t)x) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    uint32_t p = get_prime(n);
    uint32_t q = get_prime(n + 1);

    uint32_t sqrt_q = isqrt_u32(q);
    seq_t gap = (seq_t)(q - p);

    return (seq_t)sqrt_q - gap;
}
