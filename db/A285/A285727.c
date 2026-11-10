/*
 * A285727  a(1) = a(2) = 0, and for n > 2, a(n) = A252463(A252463(n)).
 * https://oeis.org/A285727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = a(2) = 0, a(n) = A252463(A252463(n)) for n > 2, where
 *            A252463(1) = 1, A252463(2k) = k, A252463(2k+1) = A064989(2k+1)
 * Range:     u8 n <= 268, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 115 terms through n = 8066 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A285727_isp(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A285727_prevprime(uint64_t p)
{
    uint64_t q = p - 1;
    while (q > 1 && !A285727_isp(q)) --q;
    return q;
}

static uint64_t A285727_f(uint64_t m)
{
    if (m <= 1) return 1;
    if ((m & 1u) == 0) return m >> 1;
    uint64_t res = 1;
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t q = A285727_prevprime(p);
            while (m % p == 0) {
                m /= p;
                res *= q;
            }
        }
    }
    if (m > 1) res *= A285727_prevprime(m);
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)0;
    uint64_t r = A285727_f(A285727_f((uint64_t)n));
    return (seq_t)r;
}
