/*
 * A346244  a(n) = n - A342001(n).
 * https://oeis.org/A346244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - A342001(n), where A342001(n) = Sum_{p^v || n}
 *            v*rad(n)/p = A003415(n)/A003557(n)
 * Range:     i8 n <= 86, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    idx_t t = n;
    idx_t primes[64];
    idx_t exps[64];
    size_t fcnt = 0;

    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            primes[fcnt] = p;
            exps[fcnt] = e;
            ++fcnt;
        }
    }
    if (t > 1) {
        primes[fcnt] = t;
        exps[fcnt] = 1;
        ++fcnt;
    }

    seq_t rad = 1;
    for (size_t i = 0; i < fcnt; ++i)
        rad *= (seq_t)primes[i];

    seq_t d = 0;
    for (size_t i = 0; i < fcnt; ++i)
        d += (seq_t)exps[i] * (rad / (seq_t)primes[i]);

    return (seq_t)n - d;
}
