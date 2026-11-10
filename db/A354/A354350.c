/*
 * A354350  a(n) = n + A354365(n).
 * https://oeis.org/A354350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + A354365(n); A354365(n) = 0 if n is not squarefree,
 *            else (-1)^omega(n) * n / gcd(n, prod_{p|n} prevprime(p))
 * Range:     u8 n <= 128, u16 n <= 12001, u32 n <= 12001, u64 n <= 12001,
 *            u128 n <= 12001, big any n
 * Validated: standard, pass, 101 terms through n = 8922 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A354350_isprime(idx_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;
    if (x % 2 == 0) return false;
    for (idx_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t ps[24];
    int cnt = 0;
    idx_t t = n;
    for (idx_t p = 2; p * p <= t; p += (p == 2 ? 1 : 2)) {
        if (t % p == 0) {
            t /= p;
            if (t % p == 0) return (seq_t)n;   /* not squarefree: A354365 = 0 */
            ps[cnt++] = p;
        }
    }
    if (t > 1) ps[cnt++] = t;

    idx_t g = 1;
    for (int i = 0; i < cnt; ++i) {
        idx_t p = ps[i];
        if (p <= 2) continue;
        idx_t q = p - 1;
        while (!A354350_isprime(q)) --q;
        for (int j = 0; j < cnt; ++j)
            if (ps[j] == q) { g *= q; break; }
    }
    idx_t x = n / g;
    if (cnt % 2 == 0) return (seq_t)n + (seq_t)x;
    return (seq_t)n - (seq_t)x;
}
