/*
 * A305806  Möbius transform of A175851.
 * https://oeis.org/A305806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} mu(n/d)*A175851(d), with A175851(d)=1 for d=1
 *            or prime, else d - previousprime(d) + 1
 * Range:     i8 n <= 65537, i16 n <= 65537, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9851 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A305806_isprime(idx_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;
    if (x % 2 == 0) return false;
    for (idx_t p = 3; p <= x / p; p += 2)
        if (x % p == 0) return false;
    return true;
}

static int A305806_mu(idx_t n)
{
    int cnt = 0;
    for (idx_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return 0;
            ++cnt;
        }
    }
    if (n > 1) ++cnt;
    return (cnt & 1) ? -1 : 1;
}

static seq_t A305806_f(idx_t d)
{
    if (d < 4) return 1;
    if (A305806_isprime(d)) return 1;
    idx_t p = d - 1;
    while (!A305806_isprime(p)) --p;
    return (seq_t)(d - p) + 1;
}

static seq_t A305806_term(idx_t d, idx_t q)
{
    int m = A305806_mu(q);
    if (m == 0) return 0;
    return (m > 0) ? A305806_f(d) : -A305806_f(d);
}

seq_t a(idx_t n)
{
    seq_t s = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            s += A305806_term(d, e);
            if (e != d) s += A305806_term(e, d);
        }
    }
    return s;
}
