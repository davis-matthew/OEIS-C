/*
 * A292941  a(1) = 0, a(2) = 1, and for n > 2, a(n) = 2*a(A252463(n)) + [n ==
 *          1 (mod 6)].
 * https://oeis.org/A292941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(2) = 1, and for n > 2, a(n) = 2*a(A252463(n)) + [n
 *            == 1 (mod 6)]; unrolled as a(n) = Sum_{i} [x_i == 1 (mod 6)]*2^i
 *            + 2^L, where x_0 = n, x_{i+1} = A252463(x_i), and x_L = 2 is the
 *            first term <= 2 (no 2^L term if the chain reaches 1 directly,
 *            i.e. n = 1)
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 136, u64 n <= 312,
 *            u128 n <= 726, big any n
 * Validated: standard, pass, 93 terms through n = 1968 (2026-11-08)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t A252463_prev_prime(uint64_t n)
{
    uint64_t p = n - 1;
    while (!is_prime((idx_t)p)) --p;
    return p;
}

static uint64_t A252463_step(uint64_t n)
{
    if (n == 1) return 1;
    if ((n & 1u) == 0) return n >> 1;
    uint64_t m = n, result = 1;
    uint64_t p = 3;
    while (p <= m / p) {
        if (m % p == 0) {
            uint64_t base = A252463_prev_prime(p);
            while (m % p == 0) {
                m /= p;
                result *= base;
            }
        }
        p += 2;
    }
    if (m > 1) result *= A252463_prev_prime(m);
    return result;
}

seq_t a(idx_t n)
{
    seq_t r = 0;
    seq_t pw = 1;
    uint64_t x = (uint64_t)n;
    while (x > 2) {
        if (x % 6 == 1) r += pw;
        pw *= 2;
        x = A252463_step(x);
    }
    if (x == 2) r += pw;
    return r;
}
