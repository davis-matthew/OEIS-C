/*
 * A281979  a(n) = (A281959(n) - A037947(n))/657931.
 * https://oeis.org/A281979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sigma_25(n) - tau_26(n))/657931, where Sum_{n>=1}
 *            tau_26(n) q^n = Delta(q)*E14(q), Delta(q) = q*Product_{k>=1}
 *            (1-q^k)^24, E14(q) = 1 - 24*Sum_{n>=1} sigma_13(n) q^n
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 59,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 9853 (2026-11-08)
 * Helpers:   factor_u64, ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"
#include "../utils/mem.h"

/* sigma_k(n) for n >= 1 */
static seq_t A281979_sigmak(uint64_t n, unsigned k)
{
    if (n == 1) return (seq_t)1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)1;
        seq_t pk = ipow((seq_t)primes[i], (seq_t)k);
        seq_t cur = (seq_t)1;
        for (uint64_t e = 1; e <= exps[i]; ++e) {
            cur *= pk;
            term += cur;
        }
        result *= term;
    }
    return result;
}

/* H[j][m] = [q^m] (prod(1-q^k)^3)^j for j = 1..8; E14[m] = [q^m] E14 */
static seq_t *A281979_H[9] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
static seq_t *A281979_E14 = NULL;
static size_t A281979_M = 0;   /* number of computed coefficients (indices 0..M-1) */

static void A281979_ensure(size_t need)
{
    if (need <= A281979_M) return;
    size_t oldM = A281979_M;
    size_t newM = oldM + oldM / 2;
    if (newM < need) newM = need;
    if (newM < 64) newM = 64;

    for (int j = 1; j <= 8; ++j) {
        seq_t *p = mem_new(seq_t, newM);
        for (size_t i = 0; i < oldM; ++i) p[i] = A281979_H[j][i];
        if (A281979_H[j] != NULL) mem_free(A281979_H[j]);
        A281979_H[j] = p;
    }
    {
        seq_t *p = mem_new(seq_t, newM);
        for (size_t i = 0; i < oldM; ++i) p[i] = A281979_E14[i];
        if (A281979_E14 != NULL) mem_free(A281979_E14);
        A281979_E14 = p;
    }

    for (size_t m = oldM; m < newM; ++m) {
        /* H1 = prod(1-q^k)^3 = Sum_t (-1)^t (2t+1) q^(t(t+1)/2) */
        seq_t h1 = (seq_t)0;
        for (size_t t = 0; ; ++t) {
            size_t T = t * (t + 1) / 2;
            if (T > m) break;
            if (T == m) {
                seq_t v = (seq_t)(2 * t + 1);
                if (t & 1) h1 = (seq_t)0 - v; else h1 = v;
                break;
            }
        }
        A281979_H[1][m] = h1;

        for (int j = 2; j <= 8; ++j) {
            seq_t s = (seq_t)0;
            const seq_t *prev = A281979_H[j - 1];
            for (size_t t = 0; ; ++t) {
                size_t T = t * (t + 1) / 2;
                if (T > m) break;
                seq_t c = (seq_t)(2 * t + 1);
                if (t & 1) s -= c * prev[m - T];
                else s += c * prev[m - T];
            }
            A281979_H[j][m] = s;
        }

        if (m == 0) A281979_E14[m] = (seq_t)1;
        else A281979_E14[m] = (seq_t)0 - (seq_t)24 * A281979_sigmak((uint64_t)m, 13);
    }
    A281979_M = newM;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t nn = (size_t)n;
    A281979_ensure(nn);

    /* tau26(n) = Sum_{i=0}^{n-1} E14[i] * P24[n-1-i] */
    seq_t tau = (seq_t)0;
    const seq_t *E = A281979_E14;
    const seq_t *P = A281979_H[8];
    for (size_t i = 0; i < nn; ++i)
        tau += E[i] * P[nn - 1 - i];

    seq_t D = A281979_sigmak((uint64_t)n, 25) - tau;

    /* detect fixed-width (wrapping) seq_t versus arbitrary precision */
    seq_t x = (seq_t)1;
    int bits = 0;
    while (bits < 200) {
        seq_t y = x + x;
        if (y <= x) break;
        x = y;
        ++bits;
    }
    if (bits >= 200) {
        return D / (seq_t)657931;
    } else {
        /* wrapping arithmetic mod 2^k: exact division by odd constant */
        seq_t m = (seq_t)657931;
        seq_t two = (seq_t)2;
        seq_t inv = m;
        for (int i = 0; i < 7; ++i)
            inv = (seq_t)(inv * (seq_t)(two - (seq_t)(m * inv)));
        return (seq_t)(D * inv);
    }
}
