/*
 * A368341  Fixed points of A368207.
 * https://oeis.org/A368341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k >= 0 with A368207(k) = k (A368207(0) = 0, so a(1)
 *            = 0). This is a search over a sieve. A368207 is computed for
 *            every k <= N at once from Sum_{m>=1} q^(m^2) * P_m(q) *
 *            (1+q^m)/(1-q^m), where P_m(q) = Sum_{0<=w,x<m} q^(w*x). The
 *            factor (1+q^m)/(1-q^m) = 2/(1-q^m) - 1 is applied as a p...
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *A368341_terms = NULL;
static size_t    A368341_nterms = 0;
static uint64_t  A368341_limit = 0;

static void A368341_sieve(uint64_t N)
{
    uint64_t *acc = (uint64_t *)calloc((size_t)(N + 1), sizeof(uint64_t));
    uint32_t *W   = (uint32_t *)malloc((size_t)(N + 1) * sizeof(uint32_t));
    uint64_t m, i, w, x;

    for (m = 1; m * m <= N; ++m) {
        uint64_t b = m * m;
        uint64_t end1;
        memset(W + b, 0, (size_t)(N + 1 - b) * sizeof(uint32_t));
        /* R_m = q^(m^2) * P_m, subtract once (the "-1" part) */
        for (w = 0; w < m; ++w) {
            uint64_t base = b + w * 0;
            for (x = 0; x < m; ++x) {
                uint64_t t = base + w * x;
                if (t > N) break;
                W[t]++;
                acc[t]--;            /* modular arithmetic; final values are nonnegative */
            }
        }
        /* S_m = R_m / (1 - q^m); acc += 2*S_m */
        end1 = b + m - 1;
        if (end1 > N) end1 = N;
        for (i = b; i <= end1; ++i) acc[i] += 2u * (uint64_t)W[i];
        for (i = b + m; i <= N; ++i) {
            W[i] += W[i - m];
            acc[i] += 2u * (uint64_t)W[i];
        }
    }

    A368341_nterms = 0;
    free(A368341_terms);
    A368341_terms = (uint64_t *)malloc((size_t)(N + 1) * sizeof(uint64_t));
    for (i = 0; i <= N; ++i)
        if (acc[i] == i) A368341_terms[A368341_nterms++] = i;

    A368341_limit = N;
    free(acc);
    free(W);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A368341_nterms < n) {
        uint64_t L = A368341_limit ? A368341_limit * 2 : 1024;
        A368341_sieve(L);
    }
    return (seq_t)A368341_terms[n - 1];
}
