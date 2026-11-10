/*
 * A266408  Permutation of natural numbers: a(n) = (1/2) *
 *          (1+A263273(A003961(n))).
 * https://oeis.org/A266408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2) * (1+A263273(A003961(n)))
 * Range:     u8 n <= 47, u16 n <= 1535, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 98 terms through n = 7922 (2026-11-07)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"

static seq_t A003961_s(idx_t n) {
    seq_t result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            while (m % p == 0) {
                m /= p;
                result *= (seq_t)q;
            }
        }
    }
    if (m > 1) {
        result *= (seq_t)next_prime(m);
    }
    return result;
}

static seq_t A263273_s(seq_t n) {
    if (n == 0)
        return 0;
    seq_t temp = n;
    seq_t p3 = 1;
    while ((temp % 3) == 0) {
        temp /= 3;
        p3 *= 3;
    }
    seq_t rev = 0;
    seq_t m = temp;
    while (m > 0) {
        rev = rev * 3 + (m % 3);
        m /= 3;
    }
    return rev * p3;
}

seq_t a(idx_t n)
{
    seq_t t = A263273_s(A003961_s(n));
    return (t + 1) / 2;
}
