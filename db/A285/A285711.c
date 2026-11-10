/*
 * A285711  a(n) = gcd(A051953(n), A079277(n)), a(1) = 1.
 * https://oeis.org/A285711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = gcd(n - phi(n), A079277(n)) for n > 1, where
 *            A079277(n) is the largest k < n with every prime factor of k
 *            dividing n (found by downward search)
 * Range:     u8 n <= 494, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 131 terms through n = 9999 (2026-11-08)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

static idx_t A051953_phi(idx_t m)
{
    idx_t result = m;
    for (idx_t d = 2; d <= m / d; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            result -= result / d;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

static idx_t A079277_a(idx_t n)
{
    idx_t primes[64];
    int pcount = 0;
    idx_t tmp = n;
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            primes[pcount++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) primes[pcount++] = tmp;

    for (idx_t k = n - 1; k > 0; --k) {
        idx_t v = k;
        for (int i = 0; i < pcount; ++i) {
            idx_t p = primes[i];
            while (v % p == 0) v /= p;
        }
        if (v == 1) return k;
    }
    return 1;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    seq_t c = (seq_t)(n - A051953_phi(n));
    seq_t b = (seq_t)A079277_a(n);
    return gcd(c, b);
}
