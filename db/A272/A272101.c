/*
 * A272101  Square root of largest square dividing A069482(n).
 * https://oeis.org/A272101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000188(A069482(n)), where A069482(n) = prime(n+1)^2 -
 *            prime(n)^2 and A000188(m) is the square root of the largest
 *            square dividing m
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t *a272101_pr = NULL;
static size_t a272101_n = 0, a272101_cap = 0;

static void a272101_push(uint64_t p)
{
    if (a272101_n >= a272101_cap) {
        size_t nc = a272101_cap ? a272101_cap * 2 : 64;
        uint64_t *t = (uint64_t *)realloc(a272101_pr, nc * sizeof(uint64_t));
        if (!t) abort();
        a272101_pr = t;
        a272101_cap = nc;
    }
    a272101_pr[a272101_n++] = p;
}

static void a272101_ensure(size_t need)
{
    if (a272101_n == 0) a272101_push(2);
    uint64_t c = a272101_pr[a272101_n - 1] + 1;
    if (c % 2 == 0) c++;
    while (a272101_n < need) {
        int ok = 1;
        for (size_t i = 0; i < a272101_n; i++) {
            uint64_t p = a272101_pr[i];
            if (p * p > c) break;
            if (c % p == 0) { ok = 0; break; }
        }
        if (ok) a272101_push(c);
        c += 2;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    a272101_ensure((size_t)n + 1);
    uint64_t p = a272101_pr[n - 1], q = a272101_pr[n];
    uint64_t m = (q - p) * (q + p);
    seq_t r = 1;
    for (uint64_t f = 2; f <= m / f; f += (f == 2 ? 1 : 2)) {
        if (m % f == 0) {
            unsigned e = 0;
            while (m % f == 0) { m /= f; e++; }
            for (unsigned k = e / 2; k > 0; k--) r *= (seq_t)f;
        }
    }
    return r;
}
