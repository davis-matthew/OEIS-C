/*
 * A325300  a(n) is the number of faces of the stepped pyramid with n levels
 *          described in A245092.
 * https://oeis.org/A325300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = a(n-1) + A237271(n) + 2*V(n), a(0) = 3. * The pyramid is
 *            built from the Dyck paths of A237593. Level n is the slab *
 *            under path n. The 3 is for the bottom face and the two flat back
 *            walls. * A237271(n) is the number of new horizontal faces: the
 *            connected parts of * the region between path...
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct { uint64_t x0, x1, y; } A325300_RSeg;
typedef struct { uint64_t x, ylo, yhi; } A325300_DSeg;

/* Build Dyck path n of A237593: from (0,n) to (n,0), alternating right/down. */
static void A325300_build_path(uint64_t n, A325300_RSeg **R, size_t *nr,
                               A325300_DSeg **D, size_t *nd)
{
    *nr = 0; *nd = 0; *R = NULL; *D = NULL;
    if (n == 0) return;
    uint64_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    uint64_t *H = (uint64_t *)malloc((size_t)(r + 2) * sizeof(uint64_t));
    uint64_t *w = (uint64_t *)malloc((size_t)(r + 1) * sizeof(uint64_t));
    for (uint64_t k = 1; k <= r; k++) {
        uint64_t num = 2 * n + 2 - k * (k + 1);
        H[k] = (num + 2 * k - 1) / (2 * k);
    }
    for (uint64_t k = 1; k < r; k++) w[k - 1] = H[k] - H[k + 1];
    w[r - 1] = H[r];
    *R = (A325300_RSeg *)malloc((size_t)(r + 1) * sizeof(A325300_RSeg));
    *D = (A325300_DSeg *)malloc((size_t)(r + 1) * sizeof(A325300_DSeg));
    uint64_t x = 0, y = n;
    for (uint64_t i = 0; i < 2 * r; i++) {
        uint64_t len = (i < r) ? w[i] : w[2 * r - 1 - i];
        if ((i & 1) == 0) {
            (*R)[*nr].x0 = x; (*R)[*nr].x1 = x + len; (*R)[*nr].y = y;
            (*nr)++; x += len;
        } else {
            (*D)[*nd].x = x; (*D)[*nd].ylo = y - len; (*D)[*nd].yhi = y;
            (*nd)++; y -= len;
        }
    }
    free(H); free(w);
}

static uint64_t A325300_delta(uint64_t n)
{
    A325300_RSeg *Rn, *Rp; A325300_DSeg *Dn, *Dp;
    size_t nrn, ndn, nrp, ndp;
    A325300_build_path(n, &Rn, &nrn, &Dn, &ndn);
    A325300_build_path(n - 1, &Rp, &nrp, &Dp, &ndp);

    /* horizontal parts between path n-1 and path n */
    uint64_t parts = 0;
    size_t i = 0, j = 0;
    uint64_t x = 0;
    bool prev = false;
    uint64_t plo = 0, phi = 0;
    while (x < n && i < nrn) {
        uint64_t hn = Rn[i].y, en = Rn[i].x1;
        uint64_t hp = (j < nrp) ? Rp[j].y : 0;
        uint64_t ep = (j < nrp) ? Rp[j].x1 : n;
        uint64_t e = en < ep ? en : ep;
        uint64_t lo = hp, hi = hn;
        if (hi > lo) {
            uint64_t mlo = lo > plo ? lo : plo;
            uint64_t mhi = hi < phi ? hi : phi;
            if (!(prev && mlo < mhi)) parts++;
            prev = true; plo = lo; phi = hi;
        } else {
            prev = false;
        }
        x = e;
        if (Rn[i].x1 == x) i++;
        if (j < nrp && Rp[j].x1 == x) j++;
    }

    /* new vertical components */
    uint64_t newv = 0;
    j = 0;
    for (size_t k = 0; k < ndn; k++) {
        while (j < ndp && Dp[j].x < Dn[k].x) j++;
        bool merged = false;
        if (j < ndp && Dp[j].x == Dn[k].x) {
            uint64_t mlo = Dn[k].ylo > Dp[j].ylo ? Dn[k].ylo : Dp[j].ylo;
            uint64_t mhi = Dn[k].yhi < Dp[j].yhi ? Dn[k].yhi : Dp[j].yhi;
            if (mlo < mhi) merged = true;
        }
        if (!merged) newv++;
    }

    free(Rn); free(Dn); free(Rp); free(Dp);
    return parts + 2 * newv;
}

static uint64_t *A325300_cache = NULL;
static size_t A325300_cnt = 0; /* cache[0..cnt-1] valid, cache[k] = a(k) */

seq_t a(idx_t n)
{
    if (A325300_cnt == 0) {
        A325300_cache = (uint64_t *)malloc(16 * sizeof(uint64_t));
        A325300_cache[0] = 3;
        A325300_cnt = 1;
    }
    if ((size_t)n >= A325300_cnt) {
        A325300_cache = (uint64_t *)realloc(A325300_cache,
                                            ((size_t)n + 1) * sizeof(uint64_t));
        if (!A325300_cache) abort();
        for (size_t k = A325300_cnt; k <= (size_t)n; k++)
            A325300_cache[k] = A325300_cache[k - 1] + A325300_delta((uint64_t)k);
        A325300_cnt = (size_t)n + 1;
    }
    return (seq_t)A325300_cache[n];
}
