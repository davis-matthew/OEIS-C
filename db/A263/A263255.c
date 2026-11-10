/*
 * A263255  Square array A(r,c), where each row r lists all numbers that are r
 *          edges distant from the infinite trunk (A259934) of the tree
 *          defined by edge-relation A049820(child) = parent.
 * https://oeis.org/A263255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   sieve-based: parent(m) = m - d(m) (A049820); trunk = path from 0
 *            through the child of greatest subtree height; A(r,c) = c-th
 *            number at distance r from the trunk, read by antidiagonals
 * Range:     u8 n <= 142, u16 n <= 10439, u32 n <= 10439, u64 n <= 10439,
 *            u128 n <= 10439, big any n
 * Validated: standard, pass, 94 terms through n = 9011 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static int *sq_dist = 0;
static long long sq_L = 0;

static void sq_build(long long L)
{
    int *d = (int *)calloc((size_t)L + 1, sizeof(int));
    int *h = (int *)calloc((size_t)L + 1, sizeof(int));
    int *bc = (int *)calloc((size_t)L + 1, sizeof(int));
    char *tr = (char *)calloc((size_t)L + 1, 1);
    int *dist = (int *)calloc((size_t)L + 1, sizeof(int));
    long long i, j, m;
    for (i = 1; i <= L; i++)
        for (j = i; j <= L; j += i) d[j]++;
    for (m = L; m >= 1; m--) {
        long long p = m - d[m];
        if (bc[p] == 0 || h[m] + 1 > h[p]) {
            if (bc[p] == 0 || h[m] + 1 > h[p]) {
                h[p] = h[m] + 1;
                bc[p] = (int)m;
            }
        }
    }
    {
        long long t = 0;
        tr[0] = 1;
        while (bc[t] != 0) {
            t = bc[t];
            tr[t] = 1;
        }
    }
    dist[0] = 0;
    for (m = 1; m <= L; m++) {
        if (tr[m]) dist[m] = 0;
        else dist[m] = dist[m - d[m]] + 1;
    }
    free(d); free(h); free(bc); free(tr);
    if (sq_dist) free(sq_dist);
    sq_dist = dist;
    sq_L = L;
}

seq_t a(idx_t n)
{
    unsigned long long k = 0;
    unsigned long long nn = (unsigned long long)n;
    long long row, col, L, m, cnt;
    while ((k + 1) * (k + 2) / 2 <= nn) k++;
    {
        unsigned long long pos = nn - k * (k + 1) / 2;
        row = (long long)pos;
        col = (long long)(k + 1 - pos);
    }
    L = sq_L ? sq_L : 4096;
    for (;;) {
        if (sq_L < L) sq_build(L);
        cnt = 0;
        for (m = 0; m <= sq_L / 4; m++) {
            if (sq_dist[m] == row) {
                cnt++;
                if (cnt == col) return (seq_t)m;
            }
        }
        L = sq_L * 2;
    }
}
