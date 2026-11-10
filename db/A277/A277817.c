/*
 * A277817  Positions of zeros in A277815; sequence A277711 sorted into
 *          ascending order.
 * https://oeis.org/A277817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (ascending) with A264977(k) not equal to
 *            A264977(j) for any j < k; A264977(0)=0, A264977(1)=1,
 *            A264977(2m)=2*A264977(m), A264977(2m+1)=A264977(m) XOR
 *            A264977(m+1). Computed by table fill, sort and dedupe
 * Range:     u8 n <= 85, u16 n <= 5619, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9781 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static const seq_t *A277817_vals;

static int A277817_cmp(const void *x, const void *y)
{
    idx_t i = *(const idx_t *)x, j = *(const idx_t *)y;
    if (A277817_vals[i] < A277817_vals[j]) return -1;
    if (A277817_vals[j] < A277817_vals[i]) return 1;
    if (i < j) return -1;
    if (i > j) return 1;
    return 0;
}

static idx_t *A277817_firsts = NULL;
static size_t A277817_cnt = 0;

static void A277817_build(size_t M)
{
    seq_t *v = mem_new(seq_t, M);
    idx_t *ord = (idx_t *)malloc(M * sizeof(idx_t));
    unsigned char *flag = (unsigned char *)calloc(M, 1);
    size_t k;
    v[0] = 0;
    if (M > 1) v[1] = 1;
    for (k = 2; k < M; k++) {
        size_t h = k >> 1;
        if ((k & 1) == 0) v[k] = v[h] * 2;
        else v[k] = v[h] ^ v[h + 1];
    }
    for (k = 0; k < M; k++) ord[k] = (idx_t)k;
    A277817_vals = v;
    qsort(ord, M, sizeof(idx_t), A277817_cmp);
    for (k = 0; k < M; k++) {
        if (k == 0 || v[ord[k - 1]] < v[ord[k]]) flag[ord[k]] = 1;
    }
    size_t c = 0;
    for (k = 0; k < M; k++) if (flag[k]) c++;
    free(A277817_firsts);
    A277817_firsts = (idx_t *)malloc((c ? c : 1) * sizeof(idx_t));
    c = 0;
    for (k = 0; k < M; k++) if (flag[k]) A277817_firsts[c++] = (idx_t)k;
    A277817_cnt = c;
    free(flag);
    free(ord);
    mem_free(v);
    A277817_vals = NULL;
}

seq_t a(idx_t n)
{
    static size_t M = 0;
    if (M == 0) { M = 1024; A277817_build(M); }
    while ((idx_t)A277817_cnt <= n) {
        M *= 2;
        A277817_build(M);
    }
    return (seq_t)A277817_firsts[n];
}
