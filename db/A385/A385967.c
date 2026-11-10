/*
 * A385967  Smallest nonnegative integer whose square is the sum of the
 *          squares of A047432(n) distinct primes.
 * https://oeis.org/A385967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least N with N^2 = sum of squares of m distinct primes, m
 *            = A047432(n) (a(1)=0 for m=0); found by a bitset DP over (primes
 *            chosen, excess over the sum of the first c prime squares)
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

static uint64_t A385967_isqrt(uint64_t x) {
    uint64_t r = (uint64_t)sqrt((double)x);
    while (r * r > x) r--;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

seq_t a(idx_t n) {
    static const uint64_t res[5] = {0, 1, 4, 5, 6};
    uint64_t k = (uint64_t)n - 1;
    uint64_t m = 8 * (k / 5) + res[k % 5];
    if (m == 0) return (seq_t)0;

    uint64_t Emax = 1024;
    for (;;) {
        /* primes needed: first m primes, plus all p with p^2 <= p_m^2 + Emax */
        uint64_t lim = 20 * m + 100;
        uint64_t pmax = 0;
        for (;;) {
            unsigned char *sv = (unsigned char *)calloc(lim + 1, 1);
            size_t cnt = 0;
            uint64_t *pr = (uint64_t *)malloc(sizeof(uint64_t) * (lim / 2 + 10));
            for (uint64_t i = 2; i <= lim; i++) {
                if (!sv[i]) {
                    pr[cnt++] = i;
                    for (uint64_t j = i * i; j <= lim; j += i) sv[j] = 1;
                }
            }
            free(sv);
            if (cnt >= m) {
                pmax = pr[m - 1];
                uint64_t need = A385967_isqrt(pmax * pmax + Emax) + 1;
                if (need <= lim) {
                    /* primes p with p^2 <= pmax^2 + Emax */
                    size_t L = 0;
                    while (L < cnt && pr[L] * pr[L] <= pmax * pmax + Emax) L++;
                    if (L < m) L = m;
                    uint64_t S0 = 0;
                    for (uint64_t i = 0; i < m; i++) S0 += pr[i] * pr[i];
                    size_t W = (size_t)(Emax >> 6) + 1;
                    uint64_t *bits = (uint64_t *)calloc((size_t)(m + 1) * W, sizeof(uint64_t));
                    bits[0] = 1;
                    for (size_t i = 0; i < L; i++) {
                        uint64_t top = (i < m - 1) ? i : m - 1;
                        for (int64_t c = (int64_t)top; c >= 0; c--) {
                            uint64_t w = pr[i] * pr[i] - pr[c] * pr[c];
                            if (w >= (uint64_t)W * 64) continue;
                            size_t ws = (size_t)(w >> 6);
                            unsigned bs = (unsigned)(w & 63);
                            uint64_t *src = bits + (size_t)c * W;
                            uint64_t *dst = bits + (size_t)(c + 1) * W;
                            for (size_t j = W; j-- > ws;) {
                                uint64_t v = src[j - ws] << bs;
                                if (bs && j - ws > 0) v |= src[j - ws - 1] >> (64 - bs);
                                dst[j] |= v;
                            }
                        }
                    }
                    uint64_t *fin = bits + (size_t)m * W;
                    uint64_t N = A385967_isqrt(S0);
                    if (N * N < S0) N++;
                    uint64_t found = 0;
                    bool ok = false;
                    for (;; N++) {
                        uint64_t d = N * N - S0;
                        if (d >= (uint64_t)W * 64) break;
                        if ((fin[d >> 6] >> (d & 63)) & 1) { found = N; ok = true; break; }
                    }
                    free(bits);
                    free(pr);
                    if (ok) return (seq_t)found;
                    break; /* enlarge Emax */
                }
            }
            free(pr);
            lim *= 2;
        }
        Emax *= 2;
    }
}
