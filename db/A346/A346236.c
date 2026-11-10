/*
 * A346236  Sum of A341530 and its Dirichlet inverse.
 * https://oeis.org/A346236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A341530(n) + A346235(n), where A341530(n) =
 *            gcd(n*sigma(A003961(n)), sigma(n)*A003961(n)) and A346235 is its
 *            Dirichlet inverse (b(1)=1, b(n) = -Sum_{d|n, d>1}
 *            A341530(d)*b(n/d))
 * Range:     i8 n <= 11, i16 n <= 209, i32 n <= 16383, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 104 terms through n = 9744 (2026-11-10)
 * Helpers:   factor_u64, gcd, is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/is_prime.h"
#include "../utils/mem.h"

static seq_t A341530_sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static seq_t A341530_a(idx_t n)
{
    seq_t N = (seq_t)n;
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exponents, &cnt);

    seq_t M = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        unsigned int e = (unsigned int)exponents[i];
        uint64_t next_p = p + 1;
        while (!is_prime(next_p))
            ++next_p;
        seq_t pow_val = 1;
        for (unsigned int k = 0; k < e; ++k)
            pow_val *= (seq_t)next_p;
        M *= pow_val;
    }

    seq_t sigma_n = A341530_sigma_of((idx_t)n);
    seq_t sigma_m = A341530_sigma_of((idx_t)M);
    seq_t term1 = N * sigma_m;
    seq_t term2 = sigma_n * M;
    return gcd(term1, term2);
}

static size_t A346236_find(const idx_t *dv, size_t cnt, idx_t v)
{
    size_t lo = 0, hi = cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (dv[mid] < v) lo = mid + 1; else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* collect divisors sorted ascending */
    size_t cap = 0;
    for (idx_t d = 1; d * d <= n; ++d)
        if (n % d == 0) cap += 2;
    idx_t *dv = mem_new(idx_t, cap + 1);
    size_t cnt = 0, small = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            dv[cnt++] = d;
            small++;
        }
    }
    for (size_t i = small; i-- > 0;) {
        idx_t d2 = n / dv[i];
        if (d2 != dv[i])
            dv[cnt++] = d2;
    }

    seq_t *fv = mem_new(seq_t, cnt);
    seq_t *bv = mem_new(seq_t, cnt);
    for (size_t i = 0; i < cnt; ++i)
        fv[i] = A341530_a(dv[i]);

    bv[0] = (seq_t)1 / fv[0]; /* f(1)=1 */
    for (size_t i = 1; i < cnt; ++i) {
        seq_t s = 0;
        for (size_t j = 1; j <= i; ++j) {
            if (dv[i] % dv[j] == 0) {
                size_t k = A346236_find(dv, cnt, dv[i] / dv[j]);
                s += fv[j] * bv[k];
            }
        }
        bv[i] = -s;
    }

    seq_t res = fv[cnt - 1] + bv[cnt - 1];
    mem_free(dv);
    mem_free(fv);
    mem_free(bv);
    return res;
}
