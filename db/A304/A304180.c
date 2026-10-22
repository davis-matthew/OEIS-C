/*
 * A304180  If n = Product (p_j^k_j) then a(n) = max{p_j}^max{k_j}.
 * https://oeis.org/A304180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (max prime factor)^(max exponent among prime factors)
 * Range:     u8 n <= 55, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n <= 1) return 1;

    uint64_t temp = (uint64_t)n;
    uint64_t maxPrime = 0;
    uint32_t maxExp   = 0;

    /* factor 2 separately */
    if (temp % 2 == 0) {
        uint32_t exp = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            exp++;
        }
        if (2 > maxPrime) maxPrime = 2;
        if (exp > maxExp) maxExp = exp;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint32_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            if (p > maxPrime) maxPrime = p;
            if (exp > maxExp) maxExp = exp;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        if (temp > maxPrime) maxPrime = temp;
        if (1 > maxExp) maxExp = 1;
    }

    return big_pow((seq_t)maxPrime, (seq_t)maxExp);
}
