/*
 * A347961  Dirichlet convolution of A342001 with itself.
 * https://oeis.org/A347961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A342001(d)*A342001(n/d), where A342001(n) =
 *            Sum_{p^v || n} v*rad(n)/p, A342001(1) = 0
 * Range:     u8 n <= 83, u16 n <= 4619, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 111 terms through n = 9566 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t A342001_a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    idx_t primes[64];
    idx_t exps[64];
    size_t fcnt = 0;

    idx_t t = n;
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            if (fcnt < 64) {
                primes[fcnt] = p;
                exps[fcnt] = e;
                ++fcnt;
            }
        }
    }
    if (t > 1 && fcnt < 64) {
        primes[fcnt] = t;
        exps[fcnt] = (idx_t)1;
        ++fcnt;
    }

    seq_t rad = (seq_t)1;
    for (size_t i = 0; i < fcnt; ++i)
        rad *= (seq_t)primes[i];

    seq_t result = (seq_t)0;
    for (size_t i = 0; i < fcnt; ++i) {
        seq_t term = rad / (seq_t)primes[i];
        result += (seq_t)exps[i] * term;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t total = (seq_t)0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0)
            continue;
        idx_t e = n / d;
        seq_t x = A342001_a(d);
        seq_t y = A342001_a(e);
        if (d == e)
            total += x * y;
        else
            total += (seq_t)2 * x * y;
    }
    return total;
}
