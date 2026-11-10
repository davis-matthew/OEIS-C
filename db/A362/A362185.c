/*
 * A362185  Numbers k with a single solution x to the equation A323410(x) = k.
 * https://oeis.org/A362185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 0 with exactly one x such that A323410(x) = k,
 *            where A323410(x) = x - prod_{p^e || x} (p^e - 1). Computed by a
 *            sieve. Write x = Q*m with Q the largest prime-power component,
 *            so k = Q*(m - phi*(m)) + phi*(m). Count solutions for every k <=
 *            K, doubling K until n terms are found. k...
 * Range:     u8 n <= 8, u16 n <= 1481, u32 n <= 2335, u64 n <= 2335,
 *            u128 n <= 2335, big any n
 * Validated: standard, pass, 64 terms through n = 2106 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *A362185_terms = NULL;
static size_t A362185_nterms = 0;

static void A362185_build(uint64_t K)
{
    uint32_t *spf = (uint32_t *)malloc((K + 1) * sizeof(uint32_t));
    uint8_t *cnt = (uint8_t *)calloc(K + 1, 1);
    uint32_t *pp = (uint32_t *)malloc((K + 1) * sizeof(uint32_t));
    uint32_t *pb = (uint32_t *)malloc((K + 1) * sizeof(uint32_t));
    size_t npp = 0;
    uint64_t i, j;

    for (i = 0; i <= K; i++) spf[i] = 0;
    for (i = 2; i <= K; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= K; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (i = 2; i <= K; i++) {
        uint64_t p = spf[i], t = i;
        while (t % p == 0) t /= p;
        if (t == 1) {
            pp[npp] = (uint32_t)i;
            pb[npp] = (uint32_t)p;
            npp++;
        }
    }
    cnt[0] = 1; /* x = 1 */

    for (uint64_t m = 2; m <= K; m++) {
        uint64_t t = m, ph = 1, maxpp = 0;
        while (t > 1) {
            uint64_t p = spf[t], pe = 1;
            while (t % p == 0) { t /= p; pe *= p; }
            ph *= (pe - 1);
            if (pe > maxpp) maxpp = pe;
        }
        uint64_t am = m - ph;
        /* first index with pp > maxpp */
        size_t lo = 0, hi = npp;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (pp[mid] > maxpp) hi = mid; else lo = mid + 1;
        }
        for (size_t q = lo; q < npp; q++) {
            uint64_t k = (uint64_t)pp[q] * am + ph;
            if (k > K) break;
            if (m % pb[q] == 0) continue;
            if (cnt[k] < 2) cnt[k]++;
        }
    }

    free(A362185_terms);
    A362185_terms = (uint64_t *)malloc((K + 1) * sizeof(uint64_t));
    A362185_nterms = 0;
    for (i = 0; i <= K; i++) {
        if (i == 1) continue;
        if (cnt[i] == 1) A362185_terms[A362185_nterms++] = i;
    }
    free(spf); free(cnt); free(pp); free(pb);
}

seq_t a(idx_t n)
{
    static uint64_t K = 0;
    if (n == 0) return 0;
    while (A362185_nterms < (size_t)n) {
        K = (K == 0) ? 4096 : K * 2;
        A362185_build(K);
    }
    return (seq_t)A362185_terms[n - 1];
}
