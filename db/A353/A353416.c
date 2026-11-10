/*
 * A353416  a(n) = A000593(A252463(n)), where A000593 is the sum of odd
 *          divisors, and A252463 is the hybrid shift.
 * https://oeis.org/A353416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000593(A252463(n)); A252463(1)=1, A252463(2k)=k,
 *            A252463(odd n)=prod prev_prime(p)^e over p^e||n; A000593 = sum
 *            of odd divisors
 * Range:     u8 n <= 262, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 103 terms through n = 9503 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A353416_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;
    if ((x & 1u) == 0) return false;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A353416_prevprime(uint64_t p)
{
    uint64_t q = p - 1;
    while (q >= 2 && !A353416_isprime(q)) --q;
    return q;
}

static uint64_t A353416_hybrid(uint64_t n)
{
    if (n == 1) return 1;
    if ((n & 1u) == 0) return n >> 1;
    uint64_t m = n, res = 1;
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t q = A353416_prevprime(p);
            while (m % p == 0) {
                m /= p;
                res *= q;
            }
        }
    }
    if (m > 1) res *= A353416_prevprime(m);
    return res;
}

seq_t a(idx_t n)
{
    uint64_t m = A353416_hybrid((uint64_t)n);
    while (m > 0 && (m & 1u) == 0) m >>= 1;
    seq_t result = 1;
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            seq_t term = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            result *= term;
        }
    }
    if (m > 1) result *= ((seq_t)m + 1);
    return result;
}
