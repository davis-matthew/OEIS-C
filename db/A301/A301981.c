/*
 * A301981  Euler transform of A034448.
 * https://oeis.org/A301981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Product_{k>=1} 1/(1-x^k)^A034448(k); computed by the Euler
 *            transform recurrence a(n) = (1/n)*Sum_{k=1..n} c(k)*a(n-k), a(0)
 *            = 1, where c(k) = Sum_{d|k} d*A034448(d)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 41, u64 n <= 104, u128 n <= 276,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 9612 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

static uint64_t A301981_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static uint64_t *A301981_c = 0;   /* c[0..A301981_cn-1] */
static size_t A301981_cn = 0;
static seq_t *A301981_b = 0;      /* computed terms b[0..A301981_bn-1] */
static size_t A301981_bn = 0;
static size_t A301981_bcap = 0;
static int A301981_bounded = -1;

static void A301981_ensure_c(size_t need)
{
    size_t m, d, k;
    uint64_t *u, *c;
    if (need <= A301981_cn) return;
    m = need;
    if (m < 2 * A301981_cn) m = 2 * A301981_cn;
    if (m < 64) m = 64;
    u = (uint64_t *)calloc(m, sizeof(uint64_t));
    c = (uint64_t *)calloc(m, sizeof(uint64_t));
    /* unitary sigma via sieve */
    for (d = 1; d < m; d++)
        for (k = d; k < m; k += d)
            if (A301981_gcd((uint64_t)d, (uint64_t)(k / d)) == 1)
                u[k] += (uint64_t)d;
    /* c(k) = sum_{d|k} d*u(d) */
    for (d = 1; d < m; d++) {
        uint64_t t = (uint64_t)d * u[d];
        for (k = d; k < m; k += d)
            c[k] += t;
    }
    free(u);
    if (A301981_c) free(A301981_c);
    A301981_c = c;
    A301981_cn = m;
}

seq_t a(idx_t n)
{
    size_t need, k, j;
    if ((uint64_t)n < (uint64_t)A301981_bn)
        return A301981_b[(size_t)n];

    if (A301981_bounded < 0) {
        seq_t z = 0;
        z -= 1;
        A301981_bounded = (z > 0) ? 1 : 0;
    }

    need = (size_t)n + 1;
    A301981_ensure_c(need);

    if (need > A301981_bcap) {
        size_t cap = 2 * A301981_bcap;
        if (cap < need) cap = need;
        if (cap < 64) cap = 64;
        if (A301981_b == 0)
            A301981_b = mem_new(seq_t, cap);
        else
            A301981_b = mem_resize(seq_t, A301981_b, cap);
        A301981_bcap = cap;
    }

    for (k = A301981_bn; k < need; k++) {
        if (k == 0) {
            A301981_b[0] = (seq_t)1;
            continue;
        }
        if (A301981_bounded) {
            /* Fixed width: split each b = q*k + r so the running sum never
               exceeds the final value a(k). */
            seq_t Q = 0;
            uint64_t R = 0;
            idx_t kk = (idx_t)k;
            for (j = 1; j <= k; j++) {
                seq_t bv = A301981_b[k - j];
                uint64_t cj = A301981_c[j];
                Q += (bv / kk) * cj;
                R += cj * (uint64_t)(bv % kk);
                if (R >= (uint64_t)kk) {
                    Q += R / (uint64_t)kk;
                    R %= (uint64_t)kk;
                }
            }
            A301981_b[k] = Q;
        } else {
            seq_t s = 0;
            for (j = 1; j <= k; j++) {
                seq_t t = A301981_b[k - j];
                t *= (idx_t)A301981_c[j];
                s += t;
            }
            A301981_b[k] = s / (idx_t)k;
        }
    }
    A301981_bn = need;
    return A301981_b[(size_t)n];
}
