/*
 * A304116  Numbers k such that A304036(k) = 2.
 * https://oeis.org/A304116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A304036(k) = 2, where A304036 has g.f.
 *            Product_{j>=1} (1 + x^(j!) + ... + x^((j+1)*j!)); computed by a
 *            saturating (cap 3) bounded-knapsack DP, with the limit doubled
 *            until enough terms are found
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 8972 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *A304116_terms = NULL;
static size_t A304116_cnt = 0;

static void A304116_build(size_t K)
{
    uint8_t *dp = (uint8_t *)calloc(K + 1, 1);
    uint8_t *tmp = (uint8_t *)calloc(K + 1, 1);
    if (!dp || !tmp) { free(dp); free(tmp); return; }
    dp[0] = 1;
    uint64_t fac = 1, j = 1;
    while (fac <= K) {
        uint64_t limit = j + 1;
        for (size_t i = 0; i <= K; ++i) tmp[i] = 0;
        for (size_t i = 0; i <= K; ++i) {
            uint8_t v = dp[i];
            if (!v) continue;
            for (uint64_t k = 0; k <= limit; ++k) {
                uint64_t idx = (uint64_t)i + fac * k;
                if (idx > K) break;
                unsigned s = tmp[idx] + v;
                tmp[idx] = (uint8_t)(s > 3 ? 3 : s);
            }
        }
        uint8_t *sw = dp; dp = tmp; tmp = sw;
        ++j;
        if (fac > K / j) break;
        fac *= j;
    }
    size_t c = 0;
    for (size_t i = 1; i <= K; ++i) if (dp[i] == 2) ++c;
    uint64_t *t = (uint64_t *)malloc((c ? c : 1) * sizeof(uint64_t));
    if (t) {
        size_t m = 0;
        for (size_t i = 1; i <= K; ++i) if (dp[i] == 2) t[m++] = i;
        free(A304116_terms);
        A304116_terms = t;
        A304116_cnt = c;
    }
    free(dp);
    free(tmp);
}

seq_t a(idx_t n)
{
    size_t K = 1024;
    while ((uint64_t)A304116_cnt < n && K < ((size_t)1 << 27)) {
        size_t before = A304116_cnt;
        if (K < 4 * (size_t)n) K = 4 * (size_t)n;
        A304116_build(K);
        if (A304116_cnt < n) {
            K *= 2;
            if (A304116_cnt == before && A304116_terms == NULL) break;
        }
    }
    if (n == 0 || n > A304116_cnt) return 0;
    return (seq_t)A304116_terms[n - 1];
}
