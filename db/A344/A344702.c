/*
 * A344702  Positions k where A344695(k) is not multiplicative.
 * https://oeis.org/A344702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that
 *            gcd(sigma(x),psi(x))*gcd(sigma(k/x),psi(k/x)) !=
 *            gcd(sigma(k),psi(k)) for some coprime split k = x*(k/x); found
 *            by incremental search over k with unitary-divisor subsets
 *            (cached)
 * Range:     u8 n <= 4, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A344702_g(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static bool A344702_test(uint64_t k)
{
    uint64_t sg[16], ps[16];
    int w = 0;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t s = 1, pw = 1, pp = 1;
            while (m % p == 0) {
                m /= p;
                pp = pw;
                pw *= p;
                s += pw;
            }
            sg[w] = s;
            ps[w] = pp * (p + 1);
            ++w;
        }
    }
    if (m > 1) {
        sg[w] = m + 1;
        ps[w] = m + 1;
        ++w;
    }
    if (w < 2) return false;
    uint64_t sk = 1, pk = 1;
    for (int i = 0; i < w; ++i) { sk *= sg[i]; pk *= ps[i]; }
    uint64_t f = A344702_g(sk, pk);
    uint32_t lim = (uint32_t)1 << (w - 1); /* subsets not containing last prime */
    for (uint32_t mask = 1; mask < lim; ++mask) {
        uint64_t sx = 1, px = 1;
        for (int i = 0; i < w; ++i)
            if (mask & ((uint32_t)1 << i)) { sx *= sg[i]; px *= ps[i]; }
        uint64_t sy = sk / sx, py = pk / px;
        if (A344702_g(sx, px) * A344702_g(sy, py) != f) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = 0;
    static size_t cnt = 0, cap = 0;
    static uint64_t next = 2;
    if (n == 0) return 0;
    while (cnt < (size_t)n) {
        if (A344702_test(next)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 256;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[cnt++] = next;
        }
        ++next;
    }
    return (seq_t)terms[n - 1];
}
