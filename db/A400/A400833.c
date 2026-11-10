/*
 * A400833  a(n) = Product_{d|n, d squarefree} A276086(A400173(n/d)), where
 *          A400173 is Dirichlet convolution of arithmetic derivative with
 *          Liouville's lambda and A276086 is the primorial base exp-function.
 * https://oeis.org/A400833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{d|n, d squarefree} A276086(A400173(n/d)),
 *            A400173(m) = Sum_{e|m} A003415(e)*lambda(m/e)
 * Range:     u8 n <= 11, u16 n <= 15, u32 n <= 47, u64 n <= 167,
 *            u128 n <= 629, big any n
 * Validated: standard, pass, 74 terms through n = 8760 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t ad_u64(uint64_t x)
{
    /* arithmetic derivative */
    if (x < 2) return 0;
    uint64_t res = 0, m = x;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            res += (x / p) * e;
        }
    }
    if (m > 1) res += x / m;
    return res;
}

static int liouville_u64(uint64_t x)
{
    int c = 0;
    for (uint64_t p = 2; p * p <= x; p++) {
        while (x % p == 0) { x /= p; c++; }
    }
    if (x > 1) c++;
    return (c & 1) ? -1 : 1;
}

static bool sqfree_u64(uint64_t x)
{
    for (uint64_t p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            x /= p;
            if (x % p == 0) return false;
        }
    }
    return true;
}

static int64_t conv_A400173(uint64_t m)
{
    int64_t s = 0;
    for (uint64_t e = 1; e * e <= m; e++) {
        if (m % e == 0) {
            uint64_t f = m / e;
            s += (int64_t)ad_u64(e) * liouville_u64(f);
            if (f != e) s += (int64_t)ad_u64(f) * liouville_u64(e);
        }
    }
    return s;
}

static seq_t primexp_A276086(uint64_t m)
{
    static const uint64_t pr[15] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};
    uint64_t prim[15];
    int maxk = 0;
    prim[0] = 1;
    for (int k = 1; k < 15; k++) {
        if (prim[k-1] > UINT64_MAX / pr[k-1]) break;
        prim[k] = prim[k-1] * pr[k-1];
        if (prim[k] > m) break;
        maxk = k;
    }
    seq_t result = 1;
    uint64_t rem = m;
    for (int k = maxk; k >= 0; k--) {
        uint64_t digit = rem / prim[k];
        rem -= digit * prim[k];
        for (uint64_t i = 0; i < digit; i++) result *= (seq_t)pr[k];
    }
    return result;
}

static seq_t term_for(uint64_t d, uint64_t n)
{
    int64_t v = conv_A400173(n / d);
    if (v < 0) v = 0;
    return primexp_A276086((uint64_t)v);
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    seq_t result = 1;
    for (uint64_t d = 1; d * d <= N; d++) {
        if (N % d == 0) {
            uint64_t d2 = N / d;
            if (sqfree_u64(d)) result *= term_for(d, N);
            if (d2 != d && sqfree_u64(d2)) result *= term_for(d2, N);
        }
    }
    return result;
}
