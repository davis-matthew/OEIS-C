/*
 * A263257  a(n) = least number with distance n to the infinite trunk
 *          (A259934) of the tree defined by edge-relation A049820(child) =
 *          parent.
 * https://oeis.org/A263257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with A263254(k) = n; computed by sieve:
 *            f(k)=k-d(k) (A049820), trunk = path from 0 following the child
 *            with the largest subtree, then least k at each distance
 * Range:     u8 n <= 32, u16 n <= 1528, u32 n <= 1835, u64 n <= 1835,
 *            u128 n <= 1835, big any n
 * Validated: standard, pass, 80 terms through n = 1756 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A263257_N 300000u

static uint32_t *A263257_first = 0;

static void A263257_init(void)
{
    uint32_t N = A263257_N, i, j;
    uint32_t *d = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    uint32_t *cnt = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    uint32_t *best = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    unsigned char *tr = (unsigned char *)calloc(N + 1, 1);
    uint32_t *dist = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    uint32_t *first = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    uint32_t t;

    for (i = 1; i <= N; i++)
        for (j = i; j <= N; j += i) d[j]++;
    for (i = 0; i <= N; i++) { cnt[i] = 1; first[i] = 0xFFFFFFFFu; }

    for (i = N; i >= 1; i--) {
        uint32_t p = i - d[i];
        cnt[p] += cnt[i];
        if (best[p] == 0 || cnt[i] > cnt[best[p]]) best[p] = i;
    }
    t = 0;
    for (;;) {
        tr[t] = 1;
        if (best[t] == 0) break;
        t = best[t];
    }
    dist[0] = 0;
    first[0] = 0;
    for (i = 1; i <= N; i++) {
        if (tr[i]) dist[i] = 0;
        else dist[i] = dist[i - d[i]] + 1;
        if (first[dist[i]] == 0xFFFFFFFFu) first[dist[i]] = i;
    }
    free(d); free(cnt); free(best); free(tr); free(dist);
    A263257_first = first;
}

seq_t a(idx_t n)
{
    if (!A263257_first) A263257_init();
    if (n > A263257_N) return 0;
    if (A263257_first[n] == 0xFFFFFFFFu) return 0;
    return (seq_t)A263257_first[n];
}
