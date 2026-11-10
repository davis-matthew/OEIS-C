/*
 * A292255  a(1) = 0, and for n > 1, a(n) = 2*a(A252463(n)) + [n == 1 (mod
 *          2)]*[J(3|n) == -1], where J is the Jacobi-symbol.
 * https://oeis.org/A292255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, a(n) = 2*a(A252463(n)) + [n == 1 (mod
 *            2)]*[J(3|n) == -1]. Here J(3|n) = -1 exactly when n is odd and n
 *            == 5 or 7 (mod 12). Unrolled: a(n) = Sum_{i>=0} 2^i * [x_i == 5
 *            or 7 (mod 12)], where x_0 = n, x_{i+1} = A252463(x_i), and the
 *            sum runs while x_i > 1
 * Range:     u8 n <= 30, u16 n <= 66, u32 n <= 148, u64 n <= 330,
 *            u128 n <= 738, big any n
 * Validated: standard, pass, 102 terms through n = 1977 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool a292255_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t a292255_prevprime(uint64_t p)
{
    uint64_t q = p - 1;
    while (!a292255_isprime(q)) --q;
    return q;
}

/* A252463: halve even numbers; for odd numbers, replace each prime factor by the previous prime */
static uint64_t a292255_step(uint64_t n)
{
    if (n == 1) return 1;
    if ((n & 1u) == 0) return n >> 1;
    uint64_t m = n, r = 1;
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t b = a292255_prevprime(p);
            while (m % p == 0) { m /= p; r *= b; }
        }
    }
    if (m > 1) r *= a292255_prevprime(m);
    return r;
}

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t pw = 1;
    uint64_t x = (uint64_t)n;
    while (x > 1) {
        unsigned r = (unsigned)(x % 12);
        if ((x & 1u) && (r == 5 || r == 7))
            res += pw;
        pw *= 2;
        x = a292255_step(x);
    }
    return res;
}
