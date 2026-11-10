/*
 * A359774  Parity of A359773, where A359773 is the Dirichlet inverse of
 *          A356163.
 * https://oeis.org/A359774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A359773(n) mod 2, where A359773 is the Dirichlet inverse
 *            of A356163: g(1)=1, g(n) = -Sum_{d|n, d<n} A356163(n/d)*g(d);
 *            computed mod 2 over the divisors of n
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 137 terms through n = 9494 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static int A356163_f(uint64_t t)
{
    if (t == 1)
        return 1;
    bool parity = false;
    while ((t & 1ULL) == 0)
        t >>= 1;
    for (uint64_t i = 3; i <= t / i; i += 2) {
        while (t % i == 0) {
            parity = !parity;
            t /= i;
        }
    }
    if (t > 1)
        parity = !parity;
    return parity ? 0 : 1;
}

static int A359774_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a > b) - (a < b);
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    size_t cap = 64, cnt = 0;
    uint64_t *d = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (uint64_t i = 1; i <= N / i; i++) {
        if (N % i == 0) {
            if (cnt + 2 > cap) {
                cap *= 2;
                d = (uint64_t *)realloc(d, cap * sizeof(uint64_t));
            }
            d[cnt++] = i;
            if (i != N / i)
                d[cnt++] = N / i;
        }
    }
    qsort(d, cnt, sizeof(uint64_t), A359774_cmp);
    unsigned char *g = (unsigned char *)malloc(cnt);
    for (size_t i = 0; i < cnt; i++) {
        if (i == 0) {
            g[0] = 1;
            continue;
        }
        unsigned char s = 0;
        for (size_t j = 0; j < i; j++) {
            if (g[j] && d[i] % d[j] == 0)
                s ^= (unsigned char)A356163_f(d[i] / d[j]);
        }
        g[i] = s;
    }
    seq_t r = (seq_t)g[cnt - 1];
    free(d);
    free(g);
    return r;
}
