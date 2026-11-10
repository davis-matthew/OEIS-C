/*
 * A324333  Numbers d such that A324331(x) = d^2 only when x is a squarefree
 *          semiprime (A006881).
 * https://oeis.org/A324333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: d is a term iff no x outside A006881 has A324331(x) =
 *            (x-1)^2 - phi(x)*sigma(x) = d^2 (for x = p*q, A324331(x) =
 *            (q-p)^2). A sieve finds every non-semiprime x whose value is a
 *            square d^2 with d <= D. Values for such x grow at least like
 *            x^(4/3), so x <= ~D^1.5 is enough
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-08)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isqrt_u64.h"

static uint64_t *g324333_good = 0;
static size_t g324333_cnt = 0;
static uint64_t g324333_D = 0;

static void g324333_build(uint64_t D)
{
    uint64_t L = 4 * D * (isqrt_u64(D) + 1) + 1000;
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    unsigned char *bad = (unsigned char *)calloc((size_t)D + 2, 1);
    uint64_t i, j;
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= L)
                for (j = i * i; j <= L; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (i = 2; i <= L; i++) {
        uint64_t m = i, phi = 1, sig = 1;
        int om = 0;
        bool sqf = true;
        while (m > 1) {
            uint64_t p = spf[m], pk = 1, sum = 1;
            int e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
                pk *= p;
                sum += pk;
            }
            om++;
            if (e > 1) sqf = false;
            phi *= (pk / p) * (p - 1);
            sig *= sum;
        }
        if (om == 2 && sqf) continue;
        {
            uint64_t A = (i - 1) * (i - 1), B = phi * sig;
            if (A > B) {
                uint64_t v = A - B;
                uint64_t r = isqrt_u64(v);
                if (r * r == v && r <= D) bad[r] = 1;
            }
        }
    }
    free(g324333_good);
    g324333_good = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(D + 1));
    g324333_cnt = 0;
    for (uint64_t d = 1; d <= D; d++)
        if (!bad[d]) g324333_good[g324333_cnt++] = d;
    g324333_D = D;
    free(spf);
    free(bad);
}

seq_t a(idx_t n)
{
    if (g324333_D == 0) g324333_build(64);
    while ((uint64_t)g324333_cnt < (uint64_t)n)
        g324333_build(g324333_D * 2);
    return (seq_t)g324333_good[(size_t)(n - 1)];
}
