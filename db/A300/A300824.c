/*
 * A300824  Difference between A300222(n) and its Möbius transform.
 * https://oeis.org/A300824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = -Sum_{d|n, d<n} A008683(n/d)*A300222(d)
 * Range:     i8 n <= 309, i16 n <= 65537, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8665 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A300222_a(idx_t n)
{
    seq_t result = 0;
    seq_t p3 = 1;
    while (n > 0)
    {
        idx_t digit = n % 3;
        n /= 3;
        if (digit == 2)
            result += (seq_t)2 * p3;
        p3 *= (seq_t)3;
    }
    return result;
}

static int A008683_a(idx_t n)
{
    if (n == 1) return 1;
    idx_t tmp = n;
    int cnt = 0;
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            ++cnt;
            tmp /= p;
            if (tmp % p == 0) return 0;
        }
    }
    if (tmp > 1) ++cnt;
    return (cnt & 1) ? -1 : 1;
}

static seq_t term(idx_t n, idx_t d)
{
    /* contribution of divisor d (d < n): -mu(n/d)*f(d) */
    int mu = A008683_a(n / d);
    if (mu == 0) return (seq_t)0;
    seq_t f = A300222_a(d);
    return (mu > 0) ? -f : f;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0) continue;
        idx_t j = n / i;
        if (i < n) sum += term(n, i);
        if (j != i && j < n) sum += term(n, j);
    }
    return sum;
}
