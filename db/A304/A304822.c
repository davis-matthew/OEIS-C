/*
 * A304822  a(n) = A304821(n+3)/A304821(n) - 1.
 * https://oeis.org/A304822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)/gcd(A304821(n), n+1), where A304821(n) =
 *            6*Product_{t=n-3,n-6,...,>=1} (1+a(t)), because A304821(n+3) =
 *            A304821(n)*(1 + (n+1)/gcd(A304821(n), n+1)); A304821(n) is
 *            tracked mod n+1
 * Range:     u8 n <= 255, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 88 terms through n = 8561 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static uint64_t A304822_gcd64(uint64_t x, uint64_t y)
{
    while (y != 0) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    static uint64_t *v = NULL;
    static size_t cap = 0;
    static size_t computed = 0;
    size_t N = (size_t)n;

    if (N + 1 > cap) {
        size_t nc = N + 1;
        if (nc < 2 * cap) nc = 2 * cap;
        uint64_t *nv = (uint64_t *)realloc(v, nc * sizeof(uint64_t));
        if (nv == NULL) return (seq_t)0;
        v = nv;
        cap = nc;
    }
    for (size_t j = computed + 1; j <= N; ++j) {
        uint64_t m = (uint64_t)j + 1;
        uint64_t r = 6 % m;
        for (size_t t = (j > 3 ? j - 3 : 0); t >= 1; t = (t > 3 ? t - 3 : 0)) {
            r = (r * ((1 + v[t]) % m)) % m;
        }
        uint64_t g = A304822_gcd64(r, m); /* gcd(0, m) = m */
        v[j] = m / g;
        computed = j;
    }
    return (seq_t)v[N];
}
