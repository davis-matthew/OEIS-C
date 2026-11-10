/*
 * A272409  Primes p == 1 (mod 3) for which A261029(46*p) = 2.
 * https://oeis.org/A272409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th prime p == 1 (mod 3) with A261029(46p) = 2; computed
 *            by search, writing x^3+y^3+z^3-3xyz = (x+y+z)(u^2+uv+v^2) with
 *            y=x+u, z=x+u+v
 * Range:     u8 n <= 25, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A272409_count(uint64_t N)
{
    uint64_t cnt = 0;
    for (uint64_t u = 0; u * u <= N; ++u) {
        for (uint64_t v = 0; ; ++v) {
            uint64_t d = u * u + u * v + v * v;
            if (d > N) break;
            if (d == 0) continue;
            if (N % d) continue;
            uint64_t s = N / d;
            uint64_t sub = 2 * u + v;
            if (s >= sub && (s - sub) % 3 == 0) ++cnt;
        }
    }
    return cnt;
}

static bool A272409_isprime(uint64_t p)
{
    if (p < 2) return false;
    for (uint64_t d = 2; d * d <= p; ++d)
        if (p % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    static uint64_t terms[200];
    static int nterms = 0;
    static bool done = false;
    if (!done) {
        for (uint64_t p = 7; p < 2116; p += 3) {
            if (!A272409_isprime(p)) continue;
            if (A272409_count(46 * p) == 2) terms[nterms++] = p;
        }
        done = true;
    }
    if (n < 1 || n > (idx_t)nterms) return (seq_t)0;
    return (seq_t)terms[n - 1];
}
