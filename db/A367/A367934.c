/*
 * A367934  a(n) is the smallest multiple of n that is an exponentially evil
 *          number (A262675).
 * https://oeis.org/A367934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, base
 * Formula:   a(n) = n * Π_{p^e||n} p^(s(e)-e) where s(e) = smallest integer k
 *            ≥ e with an even number of 1's in its binary expansion
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include needed utilities */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

static inline unsigned popcount_uint64(uint64_t x) {
    return __builtin_popcountll(x);
}

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    uint64_t temp = nn;
    seq_t result = (seq_t)nn;

    /* handle prime factor 2 first */
    if (temp % 2 == 0) {
        uint64_t e = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            e++;
        }
        uint64_t s = e;
        while (popcount_uint64(s) & 1) s++;         /* while popcount is odd */
        uint64_t mult = s - e;
        seq_t mul = pow_uint(2, mult);
        result = mul * result;
    }

    /* trial division for odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            uint64_t s = e;
            while (popcount_uint64(s) & 1) s++;
            uint64_t mult = s - e;
            seq_t mul = pow_uint(p, mult);
            result = mul * result;
        }
    }

    /* if remaining factor >1, it is a prime */
    if (temp > 1) {
        uint64_t e = 1;
        uint64_t s = e;
        while (popcount_uint64(s) & 1) s++;
        uint64_t mult = s - e;
        seq_t mul = pow_uint(temp, mult);
        result = mul * result;
    }

    return result;
}
