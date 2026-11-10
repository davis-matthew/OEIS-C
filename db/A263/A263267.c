/*
 * A263267  Breadth-first traversal of the tree defined by the edge-relation
 *          A049820(child) = parent.
 * https://oeis.org/A263267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   breadth-first traversal of the tree rooted at 0 with parent(m) =
 *            A049820(m) = m - d(m); within a row, children are listed by
 *            parent order, then increasing value. Computed by a divisor-count
 *            sieve plus BFS
 * Range:     u8 n <= 211, u16 n <= 10425, u32 n <= 10425, u64 n <= 10425,
 *            u128 n <= 10425, big any n
 * Validated: standard, pass, 104 terms through n = 9475 (2026-11-07)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/num_divisors.h"

static uint32_t *A263267_cache = 0;
static size_t A263267_count = 0;

static int A263267_try(size_t n, size_t L, uint32_t **out, size_t *outcnt)
{
    uint32_t *d = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    uint32_t *start = (uint32_t *)calloc(L + 3, sizeof(uint32_t));
    uint32_t *kids = (uint32_t *)malloc((L + 2) * sizeof(uint32_t));
    uint32_t *fill = (uint32_t *)calloc(L + 3, sizeof(uint32_t));
    uint32_t *q = (uint32_t *)malloc((L + 2) * sizeof(uint32_t));
    size_t i, j, m, head = 0, tail = 1, B = 0;
    int ok = 1;

    for (i = 1; i <= L; i++)
        for (j = i; j <= L; j += i) d[j]++;
    for (m = 1; m <= L; m++) start[m - d[m] + 1]++;
    for (i = 0; i <= L + 1; i++) start[i + 1] += start[i];
    for (m = 1; m <= L; m++) {
        size_t p = m - d[m];
        kids[start[p] + fill[p]++] = (uint32_t)m;
    }
    while ((B / 2 + 1) * (B / 2 + 1) < L + 1) B += 2;
    B = B + 6;

    q[0] = 0;
    while (tail <= n) {
        uint32_t k;
        if (head >= tail) { ok = 0; break; }
        k = q[head++];
        if ((size_t)k + B > L) { ok = 0; break; }
        for (j = start[k]; j < start[k] + fill[k]; j++) q[tail++] = kids[j];
    }
    free(d); free(start); free(kids); free(fill);
    if (!ok) { free(q); return 0; }
    *out = q;
    *outcnt = tail;
    return 1;
}

seq_t a(idx_t n)
{
    if (n >= A263267_count) {
        size_t L = (size_t)n * 4 + 100;
        uint32_t *res = 0;
        size_t cnt = 0;
        while (!A263267_try((size_t)n, L, &res, &cnt)) L *= 2;
        free(A263267_cache);
        A263267_cache = res;
        A263267_count = cnt;
    }
    return (seq_t)A263267_cache[n];
}
