/*
 * A285708  a(n) = n / A285707(n).
 * https://oeis.org/A285708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = n / gcd(n, A079277(n)), where
 *            A079277(n) is the largest k < n with rad(k) | n (found by
 *            downward search)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9666 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static idx_t A285708_gcd(idx_t x, idx_t y)
{
    while (y) {
        idx_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static idx_t A285708_A079277(idx_t n)
{
    idx_t primes[64];
    idx_t pcount = 0;
    idx_t tmp = n;

    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            primes[pcount++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1)
        primes[pcount++] = tmp;

    for (idx_t k = n - 1; k > 0; --k) {
        idx_t v = k;
        for (idx_t i = 0; i < pcount; ++i) {
            idx_t p = primes[i];
            while (v % p == 0) v /= p;
        }
        if (v == 1)
            return k;
    }
    return 1;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)1;
    idx_t k = A285708_A079277(n);
    idx_t g = A285708_gcd(n, k);
    return (seq_t)(n / g);
}
