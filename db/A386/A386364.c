/*
 * A386364  Irregular triangle read by rows. The prime numbers corresponding
 *          to the distinct prime chains in A385871. Also a permutation of the
 *          primes.
 * https://oeis.org/A386364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   chains p -> next(p) where next(p_i) = p_j (j>i) with p_j - j =
 *            p_i + i - 2 (A385871); rows = chains ordered by smallest prime,
 *            flattened; computed by sieve + chain tracing
 * Range:     u8 n <= 40, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A386364_terms = NULL;
static uint64_t A386364_count = 0;
static uint64_t A386364_limit = 0;

static void A386364_build(uint64_t L)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)L + 1, 1);
    uint64_t *pr = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)L / 2 + 2));
    uint64_t N = 0;
    for (uint64_t i = 2; i <= L; i++) {
        if (!comp[i]) {
            pr[++N] = i;
            if (i * i <= L)
                for (uint64_t j = i * i; j <= L; j += i) comp[j] = 1;
        }
    }
    free(comp);
    int64_t *d = (int64_t *)malloc(sizeof(int64_t) * (N + 1));
    uint32_t *nx = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    unsigned char *isnext = (unsigned char *)calloc(N + 1, 1);
    for (uint64_t i = 1; i <= N; i++) d[i] = (int64_t)pr[i] - (int64_t)i;
    for (uint64_t i = 1; i <= N; i++) {
        int64_t target = (int64_t)pr[i] + (int64_t)i - 2;
        uint64_t lo = i + 1, hi = N + 1;
        while (lo < hi) {
            uint64_t mid = (lo + hi) / 2;
            if (d[mid] < target) lo = mid + 1; else hi = mid;
        }
        if (lo <= N && d[lo] == target) {
            nx[i] = (uint32_t)lo;
            isnext[lo] = 1;
        }
    }
    uint64_t *out = (uint64_t *)malloc(sizeof(uint64_t) * (N + 1));
    uint64_t cnt = 0;
    for (uint64_t h = 1; h <= N; h++) {
        if (isnext[h]) continue;
        uint64_t cur = h;
        uint64_t start = cnt;
        bool ok = true;
        for (;;) {
            out[cnt++] = pr[cur];
            if (nx[cur]) { cur = nx[cur]; continue; }
            int64_t target = (int64_t)pr[cur] + (int64_t)cur - 2;
            if (target > d[N]) ok = false;
            break;
        }
        if (!ok) { cnt = start; break; }
    }
    free(d); free(nx); free(isnext); free(pr);
    if (A386364_terms) free(A386364_terms);
    A386364_terms = out;
    A386364_count = cnt;
    A386364_limit = L;
}

seq_t a(idx_t n)
{
    if (A386364_limit == 0) A386364_build(1u << 16);
    while ((uint64_t)n > A386364_count)
        A386364_build(A386364_limit * 2);
    return (seq_t)A386364_terms[n - 1];
}
