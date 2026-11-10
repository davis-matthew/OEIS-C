/*
 * A340731  Numbers k such that A067439(k) is prime.
 * https://oeis.org/A340731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over k for which A067439(k) is prime, where A067439(k) =
 *            Sum_{e|k} mu(e)*e*T(k/e), T(m) = m^2 - Sum_{j=1..m} sigma(j)
 *            (sieved for all k up to a doubling limit)
 * Range:     u8 n <= 29, u16 n <= 3680, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9188 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static idx_t *A340731_terms = 0;
static idx_t A340731_cnt = 0;
static idx_t A340731_limit = 0;

static void A340731_build(idx_t L)
{
    uint64_t *P = (uint64_t *)calloc((size_t)L + 1, sizeof(uint64_t));
    int64_t *S = (int64_t *)calloc((size_t)L + 1, sizeof(int64_t));
    int8_t *mu = (int8_t *)malloc((size_t)L + 1);
    bool *comp = (bool *)calloc((size_t)L + 1, sizeof(bool));
    idx_t i, d, m;
    for (i = 0; i <= L; i++) mu[i] = 1;
    for (i = 2; i <= L; i++) {
        if (!comp[i]) {
            for (m = i; m <= L; m += i) {
                if (m > i) comp[m] = true;
                mu[m] = (int8_t)-mu[m];
            }
            if (i <= L / i) {
                idx_t sq = i * i;
                for (m = sq; m <= L; m += sq) mu[m] = 0;
            }
        }
    }
    free(comp);
    for (d = 1; d <= L; d++)
        for (m = d; m <= L; m += d) P[m] += d;
    for (i = 1; i <= L; i++) P[i] += P[i - 1];
    for (d = 1; d <= L; d++) {
        if (mu[d] == 0) continue;
        for (m = d, i = 1; m <= L; m += d, i++) {
            int64_t t = (int64_t)(i * i - P[i]);
            if (mu[d] > 0) S[m] += (int64_t)d * t;
            else S[m] -= (int64_t)d * t;
        }
    }
    free(A340731_terms);
    A340731_terms = (idx_t *)malloc(sizeof(idx_t) * ((size_t)L + 1));
    A340731_cnt = 0;
    for (i = 2; i <= L; i++) {
        if (S[i] > 1 && is_prime_u64((uint64_t)S[i]))
            A340731_terms[A340731_cnt++] = i;
    }
    A340731_limit = L;
    free(P); free(S); free(mu);
}

seq_t a(idx_t n)
{
    if (A340731_limit == 0) A340731_build(2048);
    while (A340731_cnt < n) A340731_build(A340731_limit * 2);
    return (seq_t)A340731_terms[n - 1];
}
