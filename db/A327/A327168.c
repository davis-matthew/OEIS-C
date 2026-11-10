/*
 * A327168  Number of common divisors of n and A276086(n), with a(0) = 1.
 * https://oeis.org/A327168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(gcd(n, A276086(n))); computed as the product over
 *            primorial-base digits d_k (prime p_k) of (min(d_k, v_{p_k}(n)) +
 *            1), a(0)=1
 * Range:     u8 n <= 16383, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 122 terms through n = 9724 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A327168_isprime(uint64_t x)
{
    uint64_t i;
    if (x < 2) return 0;
    for (i = 2; i <= x / i; i++)
        if (x % i == 0) return 0;
    return 1;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p = 2;

    if (n == 0) return 1;

    while (m > 0) {
        uint64_t d = m % p;
        m /= p;
        if (d > 0) {
            uint64_t t = (uint64_t)n;
            uint64_t e = 0;
            while (e < d && t % p == 0) {
                t /= p;
                e++;
            }
            result *= (seq_t)(e + 1);
        }
        /* next prime */
        do { p++; } while (!A327168_isprime(p));
    }
    return result;
}
