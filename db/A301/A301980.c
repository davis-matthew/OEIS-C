/*
 * A301980  Expansion of Product_{k>=1} (1 + x^k)^A065958(k).
 * https://oeis.org/A301980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] Product_{k>=1} (1 + x^k)^A065958(k), expanded
 *            factor by factor as Sum_{j>=0} binomial(A065958(k), j) x^(j*k);
 *            A065958(k) = k^2 * Product_{p|k} (1 + 1/p^2)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 63, u128 n <= 152,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 876 (2026-11-08)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

/* A065958(k) computed in idx_t (small for the k values needed) */
static idx_t A301980_b(idx_t n)
{
    idx_t t = n, res = 1;
    for (idx_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            idx_t pk = 1;
            while (t % p == 0) {
                t /= p;
                pk *= p;
            }
            /* p^(2e) + p^(2e-2) = (pk/p)^2 * (p^2 + 1) */
            idx_t q = pk / p;
            res *= q * q * (p * p + 1);
        }
    }
    if (t > 1)
        res *= t * t + 1;
    return res;
}

static seq_t *A301980_A = 0;
static idx_t A301980_cnt = 0;

seq_t a(idx_t n)
{
    if (n < A301980_cnt)
        return A301980_A[n];

    idx_t N = n;
    if (N < 2 * A301980_cnt) N = 2 * A301980_cnt;
    if (N < 64) N = 64;

    seq_t *P = mem_new(seq_t, N + 1);
    seq_t *B = mem_new(seq_t, N + 1);
    P[0] = 1;

    for (idx_t k = 1; k <= N; k++) {
        idx_t b = A301980_b(k);
        idx_t jmax = N / k;
        if (jmax > b) jmax = b;
        /* B[j] = binomial(b, j), computed exactly with gcd reduction */
        B[0] = 1;
        for (idx_t j = 1; j <= jmax; j++) {
            seq_t prev = B[j - 1];
            seq_t g = gcd(prev, (seq_t)j);
            idx_t gi = (idx_t)g;
            if (gi == 0) gi = 1;
            idx_t t = (b - j + 1) / (j / gi);
            B[j] = (prev / g) * (seq_t)t;
        }
        /* in-place multiply, descending */
        for (idx_t i = N; ; i--) {
            idx_t lim = i / k;
            if (lim > jmax) lim = jmax;
            seq_t s = P[i];
            for (idx_t j = 1; j <= lim; j++)
                s += B[j] * P[i - j * k];
            P[i] = s;
            if (i == 0) break;
        }
    }

    mem_free(B);
    if (A301980_A) mem_free(A301980_A);
    A301980_A = P;
    A301980_cnt = N + 1;
    return A301980_A[n];
}
