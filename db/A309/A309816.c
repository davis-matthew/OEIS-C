/*
 * A309816  a(n) is the 2-adic valuation of A014664(n).
 * https://oeis.org/A309816
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2-adic valuation of A014664(n), where A014664(n) =
 *            multiplicative order of 2 modulo prime(n); computed directly via
 *            a sieve and order reduction
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9856 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A309816_pr = NULL;
static size_t A309816_cnt = 0;
static uint64_t A309816_lim = 0;

static void A309816_ensure(uint64_t n)
{
    while (A309816_cnt < n) {
        uint64_t lim = A309816_lim < 1024 ? 1024 : A309816_lim * 2;
        unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
        uint64_t i, j;
        size_t c = 0;
        if (!comp) abort();
        for (i = 2; i * i <= lim; i++)
            if (!comp[i])
                for (j = i * i; j <= lim; j += i) comp[j] = 1;
        for (i = 2; i <= lim; i++) if (!comp[i]) c++;
        free(A309816_pr);
        A309816_pr = (uint32_t *)malloc(c * sizeof(uint32_t));
        if (!A309816_pr) abort();
        c = 0;
        for (i = 2; i <= lim; i++) if (!comp[i]) A309816_pr[c++] = (uint32_t)i;
        free(comp);
        A309816_cnt = c;
        A309816_lim = lim;
    }
}

static uint64_t A309816_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = r * b % m;
        b = b * b % m;
        e >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    uint64_t p, order, t, q;
    unsigned v = 0;
    if (n < 2) return (seq_t)0;
    A309816_ensure((uint64_t)n);
    p = A309816_pr[n - 1];
    order = p - 1;
    t = p - 1;
    for (q = 2; q * q <= t; q++) {
        if (t % q == 0) {
            while (t % q == 0) t /= q;
            while (order % q == 0 && A309816_powmod(2, order / q, p) == 1)
                order /= q;
        }
    }
    if (t > 1) {
        q = t;
        while (order % q == 0 && A309816_powmod(2, order / q, p) == 1)
            order /= q;
    }
    while ((order & 1) == 0) { order >>= 1; v++; }
    return (seq_t)v;
}
