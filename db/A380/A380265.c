/*
 * A380265  a(n) is the denominator of the mean value of A051903(k) at the
 *          range k = 1..n.
 * https://oeis.org/A380265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator((Sum_{k=1..n} A051903(k))/n) = n/gcd(Sum, n);
 *            A051903 computed by a sieve of maximal prime exponents
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9994 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    unsigned char *m = (unsigned char *)calloc((size_t)n + 1, 1);
    if (!m)
        return (seq_t)1;
    for (uint64_t p = 2; p <= n; ++p) {
        if (m[p] != 0)
            continue; /* composite */
        unsigned e = 1;
        uint64_t pk = p;
        for (;;) {
            for (uint64_t j = pk; j <= n; j += pk)
                if (m[j] < e)
                    m[j] = (unsigned char)e;
            if (pk > n / p)
                break;
            pk *= p;
            ++e;
        }
    }
    uint64_t s = 0;
    for (uint64_t k = 1; k <= n; ++k)
        s += m[k];
    free(m);
    uint64_t x = s, y = (uint64_t)n;
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    /* x = gcd(s, n); s >= 0 and n >= 1 so x >= 1 */
    return (seq_t)((uint64_t)n / x);
}
