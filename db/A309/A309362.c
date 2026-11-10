/*
 * A309362  Positions of 0's in A326764 (interpreted as a flat sequence).
 * https://oeis.org/A309362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   search — a(n) is the flat tetrahedral index of the n-th zero of
 *            A326764. Cell (x,y,z) = (k, n-k, t-n) gets value 0 exactly when
 *            no earlier zero, in flat order (t, then n, then k), lies on one
 *            of its 13 "queen" lines (directions in {-1,0,1}^3). So the zeros
 *            are the greedy set of non-attacking 3D q...
 * Range:     u8 n <= 19, u16 n <= 904, u32 n <= 9964, u64 n <= 9964,
 *            u128 n <= 9964, big any n
 * Validated: standard, pass, 69 terms through n = 9778 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const int A309362_DIRS[13][3] = {
    {1,0,0},{0,1,0},{0,0,1},
    {1,1,0},{1,-1,0},{1,0,1},{1,0,-1},{0,1,1},{0,1,-1},
    {1,1,1},{1,1,-1},{1,-1,1},{1,-1,-1}
};

static idx_t *A309362_zpos = NULL;   /* flat indices of zeros */
static int   *A309362_zxyz = NULL;   /* coordinates of zeros, 3 per zero */
static size_t A309362_zcnt = 0, A309362_zcap = 0;
static unsigned char *A309362_mark = NULL;
static long A309362_cap = 0;         /* max coordinate supported by mark[] */
static long A309362_next_t = 0;      /* next layer to process */
static idx_t A309362_pos = 0;        /* flat index of next cell */

/* index of the line through p with direction d (identified by its endpoint) */
static size_t A309362_line(int di, const long p[3])
{
    const int *d = A309362_DIRS[di];
    long m = -1, s[3];
    int i, face = -1, a, b;
    for (i = 0; i < 3; i++)
        if (d[i] == 1 && (m < 0 || p[i] < m)) m = p[i];
    for (i = 0; i < 3; i++) s[i] = p[i] - m * d[i];
    for (i = 0; i < 3; i++)
        if (d[i] == 1 && s[i] == 0) { face = i; break; }
    a = (face == 0) ? 1 : 0;
    b = (face == 2) ? 1 : 2;
    {
        size_t w = (size_t)A309362_cap + 1;
        return (((size_t)(di * 3 + face)) * w + (size_t)s[a]) * w + (size_t)s[b];
    }
}

static void A309362_mark_all(const long p[3])
{
    int di;
    for (di = 0; di < 13; di++) A309362_mark[A309362_line(di, p)] = 1;
}

static void A309362_grow(long t)
{
    long nc = A309362_cap ? A309362_cap : 64;
    size_t w, i;
    while (nc < t) nc *= 2;
    A309362_cap = nc;
    w = (size_t)nc + 1;
    free(A309362_mark);
    A309362_mark = (unsigned char *)calloc(39 * w * w, 1);
    for (i = 0; i < A309362_zcnt; i++) {
        long p[3];
        p[0] = A309362_zxyz[3 * i];
        p[1] = A309362_zxyz[3 * i + 1];
        p[2] = A309362_zxyz[3 * i + 2];
        A309362_mark_all(p);
    }
}

static void A309362_layer(long t)
{
    long n, k;
    if (A309362_mark == NULL || t > A309362_cap) A309362_grow(t);
    for (n = 0; n <= t; n++) {
        for (k = 0; k <= n; k++) {
            long p[3];
            int di, ok = 1;
            p[0] = k; p[1] = n - k; p[2] = t - n;
            for (di = 0; di < 13; di++)
                if (A309362_mark[A309362_line(di, p)]) { ok = 0; break; }
            if (ok) {
                if (A309362_zcnt == A309362_zcap) {
                    A309362_zcap = A309362_zcap ? 2 * A309362_zcap : 1024;
                    A309362_zpos = (idx_t *)realloc(A309362_zpos, A309362_zcap * sizeof(idx_t));
                    A309362_zxyz = (int *)realloc(A309362_zxyz, 3 * A309362_zcap * sizeof(int));
                }
                A309362_zpos[A309362_zcnt] = A309362_pos;
                A309362_zxyz[3 * A309362_zcnt] = (int)p[0];
                A309362_zxyz[3 * A309362_zcnt + 1] = (int)p[1];
                A309362_zxyz[3 * A309362_zcnt + 2] = (int)p[2];
                A309362_zcnt++;
                A309362_mark_all(p);
            }
            A309362_pos++;
        }
    }
}

seq_t a(idx_t n)
{
    while (A309362_zcnt <= (size_t)n) {
        A309362_layer(A309362_next_t);
        A309362_next_t++;
    }
    return (seq_t)A309362_zpos[n];
}
