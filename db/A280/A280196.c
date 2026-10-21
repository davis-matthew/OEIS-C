/*
 * A280196  Numbers n such that a^(n-1) == 1 (mod n^2) has no solutions with 1
 *          < a < n^2-1.
 * https://oeis.org/A280196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer that satisfies the property *
 *            a^(m-1) ≡ 1 (mod m^2) has no solutions with 1 < a < m^2–1. * For
 *            m≥1 this is equivalent to: * • every odd prime divisor p of m
 *            has gcd(m-1, p-1) ≤ 2 * • at most one odd prime divisor
 *            satisfies gcd(m-1, p-1) = 2 * Even numbers always s...
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd_u64.h"

/* Euclidean GCD for unsigned 64‑bit integers. */

/* Test whether m satisfies the minimal condition of the sequence. */
static bool good_number(uint64_t m)
{
    if (m == 1) return true;
    uint64_t m_minus_1 = m - 1;
    uint64_t temp = m;

    /* Remove all factors of 2 – they do not influence the condition. */
    if ((temp & 1) == 0) {
        do { temp >>= 1; } while ((temp & 1) == 0);
    }

    uint64_t odd_prime_with_two = 0;  /* count of primes with gcd==2 */

    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t g = gcd_u64(m_minus_1, p - 1);
            if (g > 2) return false;
            if (g == 2) odd_prime_with_two++;
            while (temp % p == 0) temp /= p;
        }
    }

    if (temp > 1) { /* remaining odd prime factor */
        uint64_t g = gcd_u64(m_minus_1, temp - 1);
        if (g > 2) return false;
        if (g == 2) odd_prime_with_two++;
    }

    return odd_prime_with_two <= 1;
}

/* Compute the n‑th term of sequence A280196. */
seq_t a(idx_t n)
{
    /* n is 1‑based; n == 0 is outside the defined range. */
    if (n == 0) return 0;

    uint64_t target = (uint64_t)n;
    uint64_t count  = 0;
    uint64_t m      = 1;

    for (;;) {
        if (good_number(m)) {
            count++;
            if (count == target) {
                return (seq_t)m;
            }
        }
        ++m;
    }
}
