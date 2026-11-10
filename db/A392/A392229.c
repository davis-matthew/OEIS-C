/*
 * A392229  Index of first occurrence of n in A391449.
 * https://oeis.org/A392229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 1 with A391449(k) = n, where A391449(k) =
 *            pi(gpf(k*(k+1))) - omega(k*(k+1)); computed by a doubling sieve
 *            search
 * Range:     u8 n <= 51, u16 n <= 2692, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 75 terms through n = 4753 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *A392229_first = NULL;
static uint32_t A392229_limit = 0;

static void A392229_build(uint32_t L)
{
    uint32_t N = L + 2, i, k;
    uint32_t *spf = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    uint32_t *g = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    uint32_t *w = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    uint32_t *pi = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    uint32_t *first = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    for (i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            uint64_t j;
            for (j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    pi[0] = 0; pi[1] = 0;
    for (i = 2; i <= N; i++) pi[i] = pi[i - 1] + (spf[i] == i ? 1 : 0);
    for (i = 2; i <= N; i++) {
        uint32_t p = spf[i], m = i / p;
        if (m == 1) { g[i] = p; w[i] = 1; }
        else { g[i] = g[m]; w[i] = w[m] + (spf[m] != p ? 1 : 0); }
    }
    for (k = 1; k <= L; k++) {
        uint32_t gg = g[k] > g[k + 1] ? g[k] : g[k + 1];
        uint32_t v = pi[gg] - (w[k] + w[k + 1]);
        if (v <= L && first[v] == 0) first[v] = k;
    }
    free(spf); free(g); free(w); free(pi);
    free(A392229_first);
    A392229_first = first;
    A392229_limit = L;
}

seq_t a(idx_t n)
{
    if (A392229_first == NULL) A392229_build(4096);
    for (;;) {
        if ((uint64_t)n <= A392229_limit && A392229_first[(uint32_t)n] != 0)
            return (seq_t)A392229_first[(uint32_t)n];
        if (A392229_limit >= (1u << 28)) return 0;
        A392229_build(A392229_limit * 2);
    }
}
