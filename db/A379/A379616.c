/*
 * A379616  Denominators of the partial sums of the reciprocals of the sum of
 *          bi-unitary divisors function (A188999).
 * https://oeis.org/A379616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator(Sum_{k=1..n} 1/A188999(k)); running sum kept
 *            as fractional part P/Q (P<Q, reduced) with overflow-safe exact
 *            reduction
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 63, u64 n <= 160, u128 n <= 312,
 *            big any n
 * Validated: standard, pass, 58 terms through n = 975 (2026-11-10)
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

static seq_t A188999_a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t sum = (seq_t)1;
        seq_t term = (seq_t)1;
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        for (uint64_t k = 1; k <= e; ++k) {
            term *= (seq_t)p;
            sum += term;
            if ((e & 1U) == 0U && k == e / 2)
                sum -= term;
        }
        result *= sum;
    }
    return result;
}

seq_t a(idx_t n)
{
    static seq_t P = 0, Q = 1;   /* fractional part P/Q, 0 <= P < Q, gcd(P,Q)=1 */
    static idx_t K = 0;
    static bool init = false;
    if (!init || n < K) {
        P = (seq_t)0;
        Q = (seq_t)1;
        K = 0;
        init = true;
    }
    while (K < n) {
        ++K;
        seq_t s = A188999_a(K);
        seq_t g = gcd(Q, s);
        seq_t qg = Q / g;
        seq_t sg = s / g;
        /* numerator np = P*sg + qg over Q*sg; gcd(np, Q*sg) = gcd(np mod g, g) */
        seq_t r = ((P % g) * (sg % g) + (qg % g)) % g;
        seq_t h = (r == (seq_t)0) ? g : gcd(g, r);
        seq_t D = qg * sg * (g / h);
        /* np/h = u*sg + (v*sg + qg)/h, where P = h*u + v */
        seq_t u = P / h;
        seq_t v = P % h;
        seq_t q1 = qg / h;
        seq_t q0 = qg % h;
        seq_t w = q1 + (v * sg + q0) / h;
        seq_t x = u * sg;            /* x < D */
        if (x >= D) x %= D;
        w %= D;
        seq_t room = D - w;
        if (x >= room) x -= room;
        else x += w;
        P = x;
        Q = D;
    }
    return Q;
}
