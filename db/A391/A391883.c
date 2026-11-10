/*
 * A391883  a(n) = A000010(n) mod A048865(n).
 * https://oeis.org/A391883
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A000010(n) mod A048865(n), where A048865(n) = pi(n) -
 *            omega(n)
 * Range:     u8 n <= 657, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9715 (2026-11-10)
 * Helpers:   omega, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/omega.h"
#include "../utils/prime_pi.h"

static seq_t A000010_a(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;
    idx_t p;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            m /= p;
            result *= (seq_t)(p - 1);
            while (m % p == 0) {
                m /= p;
                result *= (seq_t)p;
            }
        }
    }
    if (m > 1)
        result *= (seq_t)(m - 1);
    return result;
}

static seq_t A048865_a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;
    seq_t pi = prime_pi(n);
    int d = omega((uint64_t)n);
    return pi - (seq_t)d;
}

seq_t a(idx_t n)
{
    seq_t m = A048865_a(n);
    if (m == 0)
        return (seq_t)0;
    return A000010_a(n) % m;
}
