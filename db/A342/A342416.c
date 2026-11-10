/*
 * A342416  a(n) = gcd(A173557(n), A342001(n)).
 * https://oeis.org/A342416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A173557(n), A342001(n)), where A173557(n) =
 *            Product_{p|n} (p-1) and A342001(n) = Sum_{p^v || n} v*rad(n)/p,
 *            with a(1) = 1
 * Range:     u8 n <= 4410, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9510 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A342416_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    idx_t primes[64];
    idx_t exps[64];
    int cnt = 0;

    idx_t t = n;
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (t > 1) {
        primes[cnt] = t;
        exps[cnt] = 1;
        ++cnt;
    }

    seq_t rad = 1;
    seq_t phirad = 1;
    for (int i = 0; i < cnt; ++i) {
        rad *= (seq_t)primes[i];
        phirad *= (seq_t)(primes[i] - 1);
    }

    seq_t sum = 0;
    for (int i = 0; i < cnt; ++i)
        sum += (seq_t)exps[i] * (rad / (seq_t)primes[i]);

    return A342416_gcd(phirad, sum);
}
