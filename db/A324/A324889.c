/*
 * A324889  a(1) = 1, and for n > 1, a(n) = A276086(n) * a(A064989(n)).
 * https://oeis.org/A324889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = A276086(n) * a(A064989(n)) for n > 1
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 16, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Validated: standard, pass, 56 terms through n = 205 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool a324889_isp(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t a324889_prevprime(uint64_t p)
{
    uint64_t q = p - 1;
    while (!a324889_isp(q)) --q;
    return q;
}

static uint64_t a324889_nextprime(uint64_t p)
{
    uint64_t q = p + 1;
    while (!a324889_isp(q)) ++q;
    return q;
}

/* A276086: primorial base digits become exponents of successive primes */
static seq_t a324889_A276086(uint64_t n)
{
    seq_t res = 1;
    uint64_t p = 2;
    while (n > 0) {
        uint64_t d = n % p;
        n /= p;
        for (uint64_t i = 0; i < d; i++) res *= (seq_t)p;
        p = a324889_nextprime(p);
    }
    return res;
}

/* A064989: multiplicative, 2^e -> 1, p^e -> prevprime(p)^e */
static uint64_t a324889_A064989(uint64_t m)
{
    uint64_t res = 1;
    while (m % 2 == 0) m /= 2;
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t q = a324889_prevprime(p);
            while (m % p == 0) {
                m /= p;
                res *= q;
            }
        }
    }
    if (m > 1) res *= a324889_prevprime(m);
    return res;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    while (m > 1) {
        result *= a324889_A276086(m);
        m = a324889_A064989(m);
    }
    return result;
}
