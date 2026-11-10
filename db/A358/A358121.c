/*
 * A358121  Distinct values of A358085, in order of appearance.
 * https://oeis.org/A358121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th distinct value of A358085 in order of first
 *            appearance (computed by generating A358085 row by row)
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 83 terms through n = 7925 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

static idx_t *A358121_S = NULL;      /* 1-based A358085 terms */
static size_t A358121_len = 0, A358121_cap = 0;
static unsigned char *A358121_seen = NULL;
static size_t A358121_seen_cap = 0;
static idx_t *A358121_D = NULL;      /* distinct values in order */
static size_t A358121_dn = 0, A358121_dcap = 0;
static size_t A358121_scanned = 0;

static unsigned A358121_bl(idx_t x)
{
    unsigned l = 0;
    while (x) { ++l; x >>= 1; }
    return l;
}

static void A358121_reserve(size_t need)
{
    if (need + 1 > A358121_cap) {
        size_t nc = A358121_cap ? A358121_cap : 16;
        while (nc < need + 1) nc <<= 1;
        idx_t *t = (idx_t *)realloc(A358121_S, nc * sizeof(idx_t));
        if (!t) exit(1);
        A358121_S = t;
        A358121_cap = nc;
    }
    if (need + 2 > A358121_seen_cap) {
        size_t nc = A358121_seen_cap ? A358121_seen_cap : 16;
        while (nc < need + 2) nc <<= 1;
        unsigned char *t = (unsigned char *)realloc(A358121_seen, nc);
        if (!t) exit(1);
        memset(t + A358121_seen_cap, 0, nc - A358121_seen_cap);
        A358121_seen = t;
        A358121_seen_cap = nc;
    }
}

static void A358121_scan(void)
{
    while (A358121_scanned < A358121_len) {
        idx_t v = A358121_S[++A358121_scanned];
        if (!A358121_seen[v]) {
            A358121_seen[v] = 1;
            if (A358121_dn >= A358121_dcap) {
                size_t nc = A358121_dcap ? A358121_dcap * 2 : 16;
                idx_t *t = (idx_t *)realloc(A358121_D, nc * sizeof(idx_t));
                if (!t) exit(1);
                A358121_D = t;
                A358121_dcap = nc;
            }
            A358121_D[A358121_dn++] = v;
        }
    }
}

static void A358121_extend(void)
{
    size_t L = A358121_len;
    A358121_reserve(2 * L);
    unsigned counts[70];
    unsigned nxt[70];
    memset(counts, 0, sizeof(counts));
    unsigned maxl = 0;
    for (size_t i = 1; i <= L; ++i) {
        unsigned l = A358121_bl(A358121_S[i]);
        if (l > maxl) maxl = l;
        ++counts[l];
    }
    unsigned s = 0;
    for (unsigned l = 0; l <= maxl; ++l) {
        nxt[l] = s;
        s += counts[l];
    }
    for (size_t i = 1; i <= L; ++i) {
        unsigned l = A358121_bl(A358121_S[i]);
        A358121_S[L + 1 + nxt[l]++] = (idx_t)i;
    }
    A358121_len = 2 * L;
    A358121_scan();
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (A358121_len == 0) {
        A358121_reserve(1);
        A358121_S[1] = 1;
        A358121_len = 1;
        A358121_scan();
    }
    while (A358121_dn < (size_t)n) A358121_extend();
    return (seq_t)A358121_D[n - 1];
}
