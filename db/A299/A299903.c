/*
 * A299903  Partial sums of A033617.
 * https://oeis.org/A299903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A033617(k), where A033617 has g.f.
 *            (1+x)^3*(1-x+x^2)*(1+x^2)*P(x)/((1-x)^3*(1-x+x^2-x^3+x^4)*(1+x+x^2+x^3+x^4)*(1+x^3+x^6)*(1+x+...+x^6))
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 50 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static void A033617_poly_mul(const int *A, size_t degA,
                             const int *B, size_t degB,
                             int *out, size_t degOut)
{
    for (size_t i = 0; i <= degOut; ++i) out[i] = 0;
    for (size_t i = 0; i <= degA; ++i)
        for (size_t j = 0; j <= degB; ++j)
            out[i + j] += A[i] * B[j];
}

static void A033617_compute_denominator(int *D)
{
    const int f1[4] = { 1, -3, 3, -1 };
    const int f2[5] = { 1, -1, 1, -1, 1 };
    const int f3[5] = { 1, 1, 1, 1, 1 };
    const int f4[7] = { 1, 0, 0, 1, 0, 0, 1 };
    const int f5[7] = { 1, 1, 1, 1, 1, 1, 1 };

    int t1[8];
    int t2[13];
    int t3[18];
    int t4[24];

    A033617_poly_mul(f1, 3, f2, 4, t1, 3 + 4);
    A033617_poly_mul(t1, 7, f3, 4, t2, 7 + 4);
    A033617_poly_mul(t2, 11, f4, 6, t3, 11 + 6);
    A033617_poly_mul(t3, 17, f5, 6, t4, 17 + 6);

    for (size_t i = 0; i <= 23; ++i)
        D[i] = t4[i];
}

static void A033617_compute_numerator(int *N)
{
    const int f1[4] = { 1, 3, 3, 1 };
    const int f2[3] = { 1, -1, 1 };
    const int f3[3] = { 1, 0, 1 };
    const int P[17] = {
        1, 0, 1, 1, 1, 1, 2, 1, 3, 1,
        2, 1, 1, 1, 1, 0, 1
    };

    int t1[6];
    int t2[8];
    int t3[24];

    A033617_poly_mul(f1, 3, f2, 2, t1, 3 + 2);
    A033617_poly_mul(t1, 5, f3, 2, t2, 5 + 2);
    A033617_poly_mul(t2, 7, P, 16, t3, 7 + 16);

    for (size_t i = 0; i <= 23; ++i)
        N[i] = t3[i];
}

static seq_t A033617_a(idx_t n)
{
    static bool initialized = false;
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static int denom[24];
    static int numer[24];

    if (!initialized) {
        A033617_compute_denominator(denom);
        A033617_compute_numerator(numer);

        cache_cap = 32;
        cache = mem_new(seq_t, cache_cap);
        cache_size = 0;
        initialized = true;
    }

    if (n >= cache_size) {
        if ((idx_t)cache_cap <= n) {
            size_t new_cap = cache_cap;
            while ((idx_t)new_cap <= n) new_cap <<= 1;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) return 0;
            cache = tmp;
            cache_cap = new_cap;
        }

        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = 0;
            size_t maxk = (i < 24) ? (size_t)i : 23;
            for (size_t k = 1; k <= maxk; ++k) {
                seq_t term = cache[i - k];
                int coeff = denom[k];
                if (coeff < 0) {
                    sum -= term * (seq_t)(-coeff);
                } else {
                    sum += term * (seq_t)coeff;
                }
            }
            seq_t rhs = (i < 24) ? (seq_t)numer[i] : 0;
            cache[i] = rhs - sum;
        }
        cache_size = n + 1;
    }

    return cache[(size_t)n];
}

seq_t a(idx_t n)
{
    static seq_t *ps = NULL;
    static size_t cnt = 0;
    static size_t cap = 0;

    if ((size_t)n >= cnt) {
        if (cap <= (size_t)n) {
            size_t new_cap = cap ? cap : 32;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            seq_t *tmp = mem_resize(seq_t, ps, new_cap);
            if (!tmp) return 0;
            ps = tmp;
            cap = new_cap;
        }
        for (size_t i = cnt; i <= (size_t)n; ++i) {
            seq_t prev = (i > 0) ? ps[i - 1] : (seq_t)0;
            ps[i] = prev + A033617_a((idx_t)i);
        }
        cnt = (size_t)n + 1;
    }
    return ps[(size_t)n];
}
