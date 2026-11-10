/*
 * A390172  G.f. A(x) satisfies A(x) = 1 + x/(1+x^3)^2 * A(x)^3.
 * https://oeis.org/A390172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (-1)^k * binomial(2*n-5*k-1,k) *
 *            A001764(n-3*k), A001764(n) = binomial(3*n,n)/(2*n+1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 939 (2026-11-10)
 * Helpers:   binomial, gcd_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"
#include "../utils/binomial.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t m = n;
    idx_t i, k;
    seq_t *t = mem_new(seq_t, (size_t)n + 1);
    seq_t r = 1;
    seq_t pos = 0, neg = 0;
    t[0] = 1;
    for (i = 1; i <= m; i++) {
        uint64_t num = (uint64_t)3 * (3 * i - 1) * (3 * i - 2);
        uint64_t den = (uint64_t)2 * i * (2 * i + 1);
        uint64_t g = gcd_u64(num, den);
        num /= g;
        den /= g;
        r /= (seq_t)den;
        r *= (seq_t)num;
        t[i] = r;
    }
    for (k = 0; 3 * k <= n; k++) {
        seq_t b;
        seq_t term;
        if (k == 0) b = 1;
        else b = binomial(2 * n - 5 * k - 1, k);
        term = b * t[n - 3 * k];
        if (k & 1) neg += term;
        else pos += term;
    }
    mem_free(t);
    return pos - neg;
}
