/*
 * A263225  Positive values of n such that A027961(n) is divisible by
 *          A000217(n).
 * https://oeis.org/A263225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive k such that (Lucas(k+2)-3) mod (k*(k+1)/2)
 *            == 0, found by search with Fibonacci fast doubling mod T
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A263225_mm(uint64_t x, uint64_t y, uint64_t m)
{
    return (uint64_t)(((unsigned __int128)x * y) % m);
}

static void A263225_fib(uint64_t n, uint64_t m, uint64_t *f, uint64_t *g)
{
    /* returns F(n), F(n+1) mod m */
    uint64_t a = 0, b = 1 % m;
    int started = 0;
    for (int i = 63; i >= 0; --i) {
        if (started) {
            uint64_t t = (2 * b + m - a) % m;
            uint64_t c = A263225_mm(a, t, m);
            uint64_t d = (A263225_mm(a, a, m) + A263225_mm(b, b, m)) % m;
            a = c; b = d;
        }
        if ((n >> i) & 1) {
            if (started) {
                uint64_t c = (a + b) % m;
                a = b; b = c;
            } else {
                started = 1;
                a = 1 % m; b = 1 % m; /* F(1), F(2) */
            }
        }
    }
    *f = a; *g = b;
}

static int A263225_ok(uint64_t k)
{
    uint64_t T = (k % 2 == 0) ? (k / 2) * (k + 1) : k * ((k + 1) / 2);
    uint64_t f, g;
    A263225_fib(k + 2, T, &f, &g);
    /* Lucas(m) = 2F(m+1) - F(m) */
    uint64_t L = (2 * g + T - f) % T;
    return L == (3 % T);
}

seq_t a(idx_t n)
{
    static uint64_t terms[4096];
    static uint64_t cnt = 0;
    static uint64_t next = 1;
    if (n == 0) return (seq_t)0;
    while (cnt < n && cnt < 4096) {
        if (A263225_ok(next)) terms[cnt++] = next;
        ++next;
    }
    return (seq_t)terms[n - 1];
}
