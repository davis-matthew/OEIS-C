/*
 * A289396  a(n) = A288851(n)/12.
 * https://oeis.org/A289396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A288851(n)/12, where E_6 = Prod_{k>=1}
 *            (1-q^k)^A288851(k). Uses the logarithmic derivative of E_6 = 1 -
 *            504*Sum sigma_5(k) q^k: L(m) = m*sigma_5(m) + 504 *
 *            Sum_{i=1..m-1} sigma_5(i)*L(m-i), L(1) = 1, (so -q E_6'/E_6 =
 *            504 * Sum L(m) q^m), and a(n) = (42/n) * Sum_{d|n} mu(n/d) *
 *            L(d). This...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 350 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static int A008683_mu(idx_t n)
{
    if (n == 1) return 1;
    idx_t tmp = n;
    int cnt = 0;
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            ++cnt;
            tmp /= p;
            if (tmp % p == 0) return 0;
        }
    }
    if (tmp > 1) ++cnt;
    return (cnt & 1) ? -1 : 1;
}

static idx_t A289396_gcd_small(idx_t x, idx_t y)
{
    while (y) { idx_t t = x % y; x = y; y = t; }
    return x;
}

static seq_t *A289396_L = NULL;
static seq_t *A289396_s5 = NULL;
static size_t A289396_max = 0;

static void A289396_build(size_t N)
{
    if (A289396_L && N <= A289396_max) return;
    mem_free(A289396_L);
    mem_free(A289396_s5);
    A289396_L  = mem_new(seq_t, N + 1);
    A289396_s5 = mem_new(seq_t, N + 1);

    for (size_t d = 1; d <= N; ++d) {
        seq_t d5 = 1;
        for (int i = 0; i < 5; ++i) d5 *= (seq_t)d;
        for (size_t m = d; m <= N; m += d) A289396_s5[m] += d5;
    }

    for (size_t m = 1; m <= N; ++m) {
        seq_t s = (seq_t)0;
        for (size_t i = 1; i < m; ++i)
            s += A289396_s5[i] * A289396_L[m - i];
        A289396_L[m] = (seq_t)m * A289396_s5[m] + (seq_t)504 * s;
    }
    A289396_max = N;
}

seq_t a(idx_t n)
{
    A289396_build((size_t)n);

    seq_t pos = 0, neg = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d != 0) continue;
        int m = A008683_mu(n / d);
        if (m == 0) continue;
        if (m > 0) pos += A289396_L[d];
        else       neg += A289396_L[d];
    }
    seq_t T = pos - neg;              /* T = n*a(n)/42 */

    idx_t g = A289396_gcd_small((idx_t)42, n);
    idx_t q = n / g;                  /* q divides T exactly */
    return (T / (seq_t)q) * (seq_t)(42 / g);
}
