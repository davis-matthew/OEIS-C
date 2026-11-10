/*
 * A338260  a(n) is the number of nodes with depth of n in a binary tree
 *          defined as: root = 1 and a child (C) of a node (N) is such that
 *          A337978(C) = N. For nodes with two children, the smaller child is
 *          assigned as the left child and the bigger one as the right child.
 *          A child of a one-child node is assigned...
 * https://oeis.org/A338260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of m with A337979(m) = n, where f(m) = m + pi(m) -
 *            pi(m + pi(m)) (A337978); level-by-level preimage search from the
 *            root 1, using a prime-count sieve
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 79 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *A338260_pi = 0;
static uint64_t A338260_L = 0;

static void A338260_ensure(uint64_t L)
{
    if (L <= A338260_L) return;
    uint64_t nl = L * 2;
    uint32_t *p = (uint32_t *)malloc((nl + 1) * sizeof(uint32_t));
    unsigned char *c = (unsigned char *)calloc(nl + 1, 1);
    uint64_t i, j;
    for (i = 2; i * i <= nl; i++)
        if (!c[i])
            for (j = i * i; j <= nl; j += i) c[j] = 1;
    p[0] = 0;
    if (nl >= 1) p[1] = 0;
    for (i = 2; i <= nl; i++) p[i] = p[i - 1] + (c[i] ? 0 : 1);
    free(c);
    free(A338260_pi);
    A338260_pi = p;
    A338260_L = nl;
}

seq_t a(idx_t n)
{
    size_t cap = 16, cnt = 1;
    uint64_t *cur = (uint64_t *)malloc(cap * sizeof(uint64_t));
    cur[0] = 1;
    for (idx_t d = 1; d <= n; d++) {
        uint64_t mx = 0;
        size_t i;
        for (i = 0; i < cnt; i++) if (cur[i] > mx) mx = cur[i];
        A338260_ensure(3 * mx + 64);
        size_t ncap = 16, ncnt = 0;
        uint64_t *nxt = (uint64_t *)malloc(ncap * sizeof(uint64_t));
        for (i = 0; i < cnt; i++) {
            uint64_t x = cur[i];
            for (uint64_t m = x + 1; m <= 2 * x + 10; m++) {
                uint64_t pm = A338260_pi[m];
                uint64_t f = m + pm - A338260_pi[m + pm];
                if (f == x) {
                    if (ncnt == ncap) {
                        ncap *= 2;
                        nxt = (uint64_t *)realloc(nxt, ncap * sizeof(uint64_t));
                    }
                    nxt[ncnt++] = m;
                }
            }
        }
        free(cur);
        cur = nxt;
        cnt = ncnt;
        cap = ncap;
        if (cnt == 0) break;
    }
    free(cur);
    return (seq_t)cnt;
}
