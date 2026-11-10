/*
 * A258449  Numbers n such that A062234(n) = A062234(n+1) = A062234(n+2).
 * https://oeis.org/A258449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search — a(n) is the n-th index m with A062234(m-1) !=
 *            A062234(m) = A062234(m+1) = A062234(m+2) != A062234(m+3), i.e.
 *            the start of a run of exactly 3 equal terms, where A062234(m) =
 *            2*prime(m) - prime(m+1); found with a prime sieve that is
 *            enlarged as needed
 * Range:     u8 none, u16 n <= 191, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 944 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *A258449_found = NULL;
static size_t A258449_cnt = 0;
static uint64_t A258449_limit = 0;

static void A258449_build(uint64_t limit)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)limit + 1, 1);
    uint64_t *pr = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)limit / 2 + 16));
    size_t pc = 0;
    uint64_t i, j;
    if (!comp || !pr) abort();
    for (i = 2; i <= limit; i++) {
        if (!comp[i]) {
            pr[pc++] = i;
            for (j = i * i; j <= limit; j += i) comp[j] = 1;
        }
    }
    /* b[k] = A062234(k+1) = 2*pr[k] - pr[k+1], for k = 0..pc-2 */
    size_t bc = pc ? pc - 1 : 0;
    int64_t *b = (int64_t *)malloc(sizeof(int64_t) * (bc + 1));
    if (!b) abort();
    for (size_t k = 0; k < bc; k++)
        b[k] = 2 * (int64_t)pr[k] - (int64_t)pr[k + 1];

    free(A258449_found);
    A258449_found = (uint64_t *)malloc(sizeof(uint64_t) * (bc + 1));
    if (!A258449_found) abort();
    A258449_cnt = 0;
    /* run start at k (0-based), run is k..k+2, need b[k+3] to exist */
    for (size_t k = 0; k + 3 < bc; k++) {
        if (k > 0 && b[k - 1] == b[k]) continue;
        if (b[k] == b[k + 1] && b[k + 1] == b[k + 2] && b[k + 2] != b[k + 3])
            A258449_found[A258449_cnt++] = (uint64_t)(k + 1);
    }
    A258449_limit = limit;
    free(b);
    free(comp);
    free(pr);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (A258449_limit == 0) A258449_build((uint64_t)1 << 20);
    while (A258449_cnt < (size_t)n)
        A258449_build(A258449_limit * 4);
    return (seq_t)A258449_found[n - 1];
}
