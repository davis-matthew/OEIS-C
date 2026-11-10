/*
 * A324394  a(n) = A009194(A005940(1+n)), where A005940 is the Doudna sequence
 *          and A009194(n) = gcd(n,sigma(n)).
 * https://oeis.org/A324394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(D, sigma(D)) with D = A005940(1+n); D is built from
 *            the binary expansion of n and sigma(D) as a product of
 *            sigma(p^e) over its prime powers
 * Range:     u8 n <= 52, u16 n <= 916, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 122 terms through n = 9990 (2026-11-08)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t A324394_value(idx_t n)
{
    uint32_t primes[72];
    int np = 0;
    for (uint32_t c = 2; np < 72; ++c) {
        int ok = 1;
        for (int j = 0; j < np && primes[j] * primes[j] <= c; ++j)
            if (c % primes[j] == 0) { ok = 0; break; }
        if (ok) primes[np++] = c;
    }
    int ex[72];
    for (int j = 0; j < 72; ++j) ex[j] = 0;

    idx_t m = n;          /* A005940(1+n) uses n = (1+n)-1 */
    int i = 0;
    while (m > 0) {
        if (m & 1) {
            ex[i]++;
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }

    seq_t x = 1, s = 1;
    for (int j = 0; j < 72; ++j) {
        if (ex[j] == 0) continue;
        seq_t p = (seq_t)primes[j];
        seq_t term = 1, pw = 1;
        for (int k = 0; k < ex[j]; ++k) {
            pw *= p;
            term += pw;
            x *= p;
        }
        s *= term;
    }
    return gcd(x, s);
}

seq_t a(idx_t n)
{
    return A324394_value(n);
}
