/*
 * A294028  Composite numbers k such that k | 1 - A078712(k).
 * https://oeis.org/A294028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over composite k >= 4 with A078712(k) == 1 (mod k), where
 *            A078712(n) = A078712(n-3) - A078712(n-1), A078712(0) = -3,
 *            A078712(1) = 1, A078712(2) = -1. A078712(k) mod k comes from x^k
 *            mod (x^3 + x^2 - 1), computed by repeated squaring
 * Range:     u8 n <= 4, u16 n <= 439, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9977 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static uint64_t A294028_mm(uint64_t x, uint64_t y, uint64_t k)
{
    return (uint64_t)(((unsigned __int128)x * y) % k);
}

static uint64_t A294028_add(uint64_t x, uint64_t y, uint64_t k)
{
    uint64_t s = x + y;
    return s >= k ? s - k : s;
}

static uint64_t A294028_sub(uint64_t x, uint64_t y, uint64_t k)
{
    return x >= y ? x - y : x + (k - y);
}

/* r = p*q mod (t^3 + t^2 - 1), coefficients mod k:  t^3 = 1 - t^2, t^4 = t^2 + t - 1 */
static void A294028_pmul(uint64_t *r, const uint64_t *p, const uint64_t *q, uint64_t k)
{
    uint64_t c[5] = {0, 0, 0, 0, 0};
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            c[i + j] = A294028_add(c[i + j], A294028_mm(p[i], q[j], k), k);
    r[0] = A294028_sub(A294028_add(c[0], c[3], k), c[4], k);
    r[1] = A294028_add(c[1], c[4], k);
    r[2] = A294028_add(A294028_sub(c[2], c[3], k), c[4], k);
}

static bool A294028_test(uint64_t k)
{
    uint64_t res[3], base[3], e = k, v;
    res[0] = 1 % k; res[1] = 0; res[2] = 0;
    base[0] = 0; base[1] = 1 % k; base[2] = 0;
    while (e > 0) {
        if (e & 1) A294028_pmul(res, res, base, k);
        e >>= 1;
        if (e > 0) A294028_pmul(base, base, base, k);
    }
    /* A078712(k) = -3*res0 + res1 - res2 */
    v = A294028_sub(res[1], A294028_mm(3 % k, res[0], k), k);
    v = A294028_sub(v, res[2], k);
    return v == 1 % k;
}

static bool A294028_isprime(uint64_t k)
{
    uint64_t d;
    if (k < 2) return false;
    if (k % 2 == 0) return k == 2;
    if (k % 3 == 0) return k == 3;
    for (d = 5; d * d <= k; d += 6)
        if (k % d == 0 || k % (d + 2) == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static idx_t cap = 0, cnt = 0;
    static uint64_t next = 4;
    if (n < 1) return 0;
    while (cnt < n) {
        uint64_t k = next++;
        if (A294028_test(k) && !A294028_isprime(k)) {
            if (cnt >= cap) {
                idx_t ncap = cap ? cap * 2 : 256;
                uint64_t *t = (uint64_t *)realloc(terms, (size_t)ncap * sizeof(uint64_t));
                if (!t) return 0;
                terms = t;
                cap = ncap;
            }
            terms[cnt++] = k;
        }
    }
    return (seq_t)terms[n - 1];
}
