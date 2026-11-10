/*
 * A363739  a(n) is the length of the n-th run of identical terms of A349509.
 * https://oeis.org/A363739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th run of A349509. A349509(m) = 3 iff m >
 *            1 and * m^3 and 6m^2-6m+3 add without carry in base 3, else 1.
 *            Search: the * base-3 digits 0..k of m^3 and 6m^2-6m+3 depend
 *            only on m mod 3^k. So a * depth-first search over the low base-3
 *            digits of m lists every m < 3^K * with A349...
 * Range:     u8 n <= 74, u16 n <= 1958, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9433 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 A363739_u128;

static A363739_u128 A363739_pw[32];
static int A363739_K = 0;
static uint64_t *A363739_hits = 0;
static size_t A363739_hc = 0, A363739_hcap = 0;
static uint64_t *A363739_runs = 0;
static size_t A363739_rc = 0, A363739_rcap = 0;

static void A363739_push_hit(uint64_t v)
{
    if (A363739_hc == A363739_hcap)
    {
        size_t nc = A363739_hcap ? A363739_hcap * 2 : 1024;
        A363739_hits = (uint64_t *)realloc(A363739_hits, nc * sizeof(uint64_t));
        A363739_hcap = nc;
    }
    A363739_hits[A363739_hc++] = v;
}

static void A363739_push_run(uint64_t v)
{
    if (A363739_rc == A363739_rcap)
    {
        size_t nc = A363739_rcap ? A363739_rcap * 2 : 1024;
        A363739_runs = (uint64_t *)realloc(A363739_runs, nc * sizeof(uint64_t));
        A363739_rcap = nc;
    }
    A363739_runs[A363739_rc++] = v;
}

/* r < 3^k; base-3 digits 0..k of x = r^3 and y = 6r^2-6r+3 already verified */
static void A363739_dfs(A363739_u128 r, int k, A363739_u128 x, A363739_u128 y)
{
    if (r > 1 && r >= A363739_pw[k - 1])
    {
        A363739_u128 qx = x / A363739_pw[k + 1];
        A363739_u128 qy = y / A363739_pw[k + 1];
        bool ok = true;
        while (qx || qy)
        {
            if ((unsigned)(qx % 3) + (unsigned)(qy % 3) >= 3) { ok = false; break; }
            qx /= 3;
            qy /= 3;
        }
        if (ok)
            A363739_push_hit((uint64_t)r);
    }
    if (k >= A363739_K)
        return;
    {
        int d;
        A363739_u128 p = A363739_pw[k + 1];
        for (d = 0; d < 3; d++)
        {
            A363739_u128 c = r + (A363739_u128)d * A363739_pw[k];
            A363739_u128 cx = c * c * c;
            A363739_u128 cy = (A363739_u128)6 * c * c - (A363739_u128)6 * c + 3;
            unsigned dx = (unsigned)((cx / p) % 3);
            unsigned dy = (unsigned)((cy / p) % 3);
            if (dx + dy < 3)
                A363739_dfs(c, k + 1, cx, cy);
        }
    }
}

static int A363739_cmp(const void *pa, const void *pb)
{
    uint64_t x = *(const uint64_t *)pa, y = *(const uint64_t *)pb;
    return (x > y) - (x < y);
}

static void A363739_build(int K)
{
    int i;
    A363739_u128 r;
    A363739_K = K;
    A363739_pw[0] = 1;
    for (i = 1; i < 32; i++)
        A363739_pw[i] = A363739_pw[i - 1] * 3;
    A363739_hc = 0;
    A363739_rc = 0;
    for (r = 0; r < 3; r++)
    {
        A363739_u128 x = r * r * r;
        A363739_u128 y = (A363739_u128)6 * r * r - (A363739_u128)6 * r + 3;
        if ((unsigned)(x % 3) + (unsigned)(y % 3) >= 3) continue;
        if ((unsigned)((x / 3) % 3) + (unsigned)((y / 3) % 3) >= 3) continue;
        A363739_dfs(r, 1, x, y);
    }
    if (A363739_hc > 1)
        qsort(A363739_hits, A363739_hc, sizeof(uint64_t), A363739_cmp);

    {
        uint64_t B = (uint64_t)A363739_pw[K]; /* positions 1..B-1 known */
        uint64_t P = 1, L = 1;
        int v = 1;
        size_t j;
        for (j = 0; j < A363739_hc; j++)
        {
            uint64_t h = A363739_hits[j];
            if (h > P + 1)
            {
                if (v == 1) L += h - 1 - P;
                else { A363739_push_run(L); v = 1; L = h - 1 - P; }
                P = h - 1;
            }
            if (v == 3) L++;
            else { A363739_push_run(L); v = 3; L = 1; }
            P = h;
        }
        if (P + 1 <= B - 1 && v == 3)
            A363739_push_run(L);
    }
}

seq_t a(idx_t n)
{
    while ((uint64_t)A363739_rc < (uint64_t)n)
    {
        int K = A363739_K ? A363739_K + 1 : 12;
        if (K > 26)
            return (seq_t)0;
        A363739_build(K);
    }
    return (seq_t)A363739_runs[(uint64_t)n - 1];
}
