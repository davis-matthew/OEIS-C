/*
 * A368914  a(n) = 1 if there is no prime p such that p^p divides A342001(n),
 *          otherwise 0.
 * https://oeis.org/A368914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if no prime p has p^p | A342001(n), else 0; a(1) = 0.
 *            A342001(n) = Sum_{p^v || n} v * rad(n)/p, then factored by trial
 *            division
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 137 terms through n = 9987 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    uint64_t t = (uint64_t)n;
    uint64_t ps[64], es[64];
    size_t cnt = 0;
    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            ps[cnt] = p; es[cnt] = e; ++cnt;
        }
    }
    if (t > 1) { ps[cnt] = t; es[cnt] = 1; ++cnt; }

    uint64_t rad = 1;
    for (size_t i = 0; i < cnt; ++i) rad *= ps[i];
    uint64_t m = 0;
    for (size_t i = 0; i < cnt; ++i) m += es[i] * (rad / ps[i]);

    /* m >= 1 for n > 1; check p^p | m via exponent >= p */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            if (e >= p) return (seq_t)0;
        }
    }
    /* leftover m > 1 is prime with exponent 1; p^p needs p >= 2 exponent, so fine */
    return (seq_t)1;
}
