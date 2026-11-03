/*
 * A333690  a(n) = n*Product_{k=1..n} floor(k^((n-k)/(n-k+1))).
 * https://oeis.org/A333690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*Product_{k=1..n}floor(k^((n-k)/(n-k+1)))
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 23, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A333690: a(n) = n * Prod_{k=1..n} floor(k^((n-k)/(n-k+1))).
 * floor(k^(m/(m+1))) is the largest t with t^(m+1) <= k^m: estimated in floating
 * point and then settled exactly by comparing the two powers in multi-limb
 * integers (the floating-point comparison alone gets boundary cases wrong). */

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* x^e as little-endian 32-bit limbs in r (room for cap limbs); returns the length */
static size_t power(uint32_t x, uint64_t e, uint32_t *r, size_t cap)
{
    size_t len = 1;
    r[0] = 1;
    while (e--) {
        uint64_t c = 0;
        for (size_t i = 0; i < len; ++i) {
            c += (uint64_t)r[i] * x;
            r[i] = (uint32_t)c;
            c >>= 32;
        }
        if (c && len < cap) r[len++] = (uint32_t)c;
    }
    return len;
}

/* t^(m+1) <= k^m ? */
static int fits(uint64_t t, uint64_t k, uint64_t m)
{
    size_t cap = (size_t)((m + 1) * 64 / 32 + 2);
    uint32_t *x = (uint32_t *)calloc(cap, sizeof *x), *y = (uint32_t *)calloc(cap, sizeof *y);
    if (!x || !y) abort();
    size_t nx = power((uint32_t)t, m + 1, x, cap), ny = power((uint32_t)k, m, y, cap);
    int r;
    if (nx != ny) r = nx < ny;
    else {
        r = 1;
        for (size_t i = nx; i-- > 0;)
            if (x[i] != y[i]) { r = x[i] < y[i]; break; }
    }
    free(x);
    free(y);
    return r;
}

static uint64_t floor_root(uint64_t k, uint64_t m)        /* floor(k^(m/(m+1))) */
{
    if (m == 0 || k <= 1) return 1;
    uint64_t t = (uint64_t)floorl(powl((long double)k, (long double)m / (long double)(m + 1)));
    if (t < 1) t = 1;
    while (t > 1 && !fits(t, k, m)) --t;
    while (t + 1 <= k && fits(t + 1, k, m)) ++t;
    return t;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    seq_t prod = (seq_t)n;
    for (idx_t k = 2; k < n; ++k) prod *= (seq_t)floor_root((uint64_t)k, (uint64_t)(n - k));
    return prod;
}
